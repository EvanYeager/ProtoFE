#include "ProtoFEPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "ProtoFECharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Actors/GridManager.h"
#include "Actors/TileActor.h"
#include "UserWidget.h"
#include "Components/CameraControllerComponent.h"
#include "Components/Pathfinder.h" 
#include "Actors/Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/GridManager.h"
#include "Tile.h"
#include "Actors/Characters/EnemyCharacter.h"

AProtoFEPlayerController::AProtoFEPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableMouseOverEvents = true;

	CameraController = CreateDefaultSubobject<UCameraControllerComponent>(TEXT("Camera Controller"));
	Pathfinder = CreateDefaultSubobject<UPathfinder>(TEXT("Pathfinder"));
}



void AProtoFEPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	HighlightTile();
}

void AProtoFEPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// camera functions
	InputComponent->BindAxis("MoveUp", CameraController, &UCameraControllerComponent::MoveCameraUp);
	InputComponent->BindAxis("MoveRight", CameraController, &UCameraControllerComponent::MoveCameraRight);
	InputComponent->BindAction("ZoomIn", IE_Pressed, CameraController, &UCameraControllerComponent::ZoomCameraIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, CameraController, &UCameraControllerComponent::ZoomCameraOut);
	InputComponent->BindAction("Back", IE_Pressed, CameraController, &UCameraControllerComponent::SetFastSpeed);
	InputComponent->BindAction("Back", IE_Released, CameraController, &UCameraControllerComponent::SetNormalSpeed);
	InputComponent->BindAction("Select", IE_Pressed, this, &AProtoFEPlayerController::Click);
}

void AProtoFEPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridManager::StaticClass(), Temp);
	GridManager = Cast<AGridManager>(Temp[0]);

}

UUserWidget* AProtoFEPlayerController::DisplayWidget(TSubclassOf<UUserWidget> WidgetClass) 
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(this, WidgetClass);
	Widget->AddToViewport();
	return Widget;
}

void AProtoFEPlayerController::RemoveWidget(UUserWidget* Widget) 
{
	if (!Widget) return;
	Widget->RemoveFromParent();
}

APlayerCharacter* AProtoFEPlayerController::GetSelectedCharacter()
{
	return SelectedCharacter;
}

void AProtoFEPlayerController::SetSelectedCharacter(APlayerCharacter* SelectedChar)
{
	SelectedCharacter = SelectedChar;
}


void AProtoFEPlayerController::HighlightTile() 
{
	// unhighlight previously selected tile
	if (SelectedTile)
		SelectedTile->HighlightPlane->SetVisibility(false);

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit))
	{
		if (ATileActor* Tile = Cast<ATileActor>(Hit.GetActor()))
		{
			SelectedTile = Tile;
			Tile->HighlightPlane->SetVisibility(true);
			Tile->SetColor(EHighlightColor::DefaultHighlight);
			if (GetSelectedCharacter())
				Tile->SetStrength(EHighlightStrength::Strong);
		}
	}
	else SelectedTile = nullptr;
}

void AProtoFEPlayerController::Click() 
{
	if (GetSelectedCharacter())
	{
		SetSelectedCharacter(nullptr);
	}
	else
	{
		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit)) // if click hit something
		{
			if (AProtoFECharacter* CharacterClicked = Cast<AProtoFECharacter>(Hit.GetActor())) // if click hit a character
				CharacterClicked->CharacterClick();
		}
	}
}

void AProtoFEPlayerController::FocusCharacter(APlayerCharacter* Char)
{
	SetSelectedCharacter(Char);
	CameraController->FocusLocation(Char->GetActorLocation());
}

void AProtoFEPlayerController::AddHighlightedTiles(AEnemyCharacter* Char) 
{
	for (UTile* Tile : Char->MovementTiles)
	{
		// Tile->Data.TileActor->HighlightPlane->SetVisibility(true);
		// Tile->Data.TileActor->SetColor(EHighlightColor::EnemyRange);
		// Tile->Data.TileActor->SetStrength(EHighlightStrength::Strong);
		Tile->Data.TileActor->SetAsEnemyRange();
	}
	EnemyRange.Characters.Add(Char);
	EnemyRange.Tiles.Append(Char->MovementTiles);
}

void AProtoFEPlayerController::RemoveHighlightedTiles(AEnemyCharacter* Char) 
{
	// resets all enemy range data
	ResetEnemyTiles(EnemyRange.Tiles);
	EnemyRange.Characters.Remove(Char);
	// loop through all highlighted characters and breadth search to find new tiles
	for (AEnemyCharacter* C : EnemyRange.Characters)
	{
		C->BreadthSearch();
		EnemyRange.Tiles.Append(C->MovementTiles);
		for (UTile* Tile : C->MovementTiles)
		{
			// Tile->Data.TileActor->HighlightPlane->SetVisibility(true);
			// Tile->Data.TileActor->SetColor(EHighlightColor::EnemyRange);
			// Tile->Data.TileActor->SetStrength(EHighlightStrength::Strong);
			Tile->Data.TileActor->SetAsEnemyRange();
		}
	}
}

void AProtoFEPlayerController::RemoveHighlightedTiles()
{
	ResetEnemyTiles(EnemyRange.Tiles);
	EnemyRange.Characters.Empty();
}

void AProtoFEPlayerController::ResetEnemyTiles(TArray<UTile*> Tiles) 
{
	for (UTile* Tile : Tiles)
	{
		Tile->Data.TileActor->ResetAsEnemyRange();
		EnemyRange.Tiles.Remove(Tile);
	}
}

// void AProtoFEPlayerController::MoveToMouseCursor()
// {
// 	// Trace to see what is under the mouse cursor
// 	FHitResult Hit;
// 	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

// 	if (Hit.bBlockingHit)
// 	{
// 		// We hit something, move there
// 		SetNewMoveDestination(Hit.ImpactPoint);
// 	}
// }

// void AProtoFEPlayerController::SetNewMoveDestination(const FVector DestLocation)
// {
// 	APawn* const MyPawn = GetPawn();
// 	if (MyPawn)
// 	{
// 		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

// 		// We need to issue move command only if far enough in order for walk animation to play correctly
// 		if ((Distance > 120.0f))
// 		{
// 			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
// 		}
// 	}
// }

// void AProtoFEPlayerController::OnSetDestinationPressed()
// {
// 	// set flag to keep updating destination until released
// 	bMoveToMouseCursor = true;
// }

// void AProtoFEPlayerController::OnSetDestinationReleased()
// {
// 	// clear flag to indicate we should stop updating the destination
// 	bMoveToMouseCursor = false;
// }




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
#include "Components/HighlightComponent.h"
#include "Actors/Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/GridManager.h"
#include "Tile.h"
#include "Actors/Characters/EnemyCharacter.h"
#include "Interfaces/Selectable.h"
#include "Components/GridOccupyComponent.h"
#include "AI/ProtoFEAIController.h"
#include "Interfaces/Commandable.h"

AProtoFEPlayerController::AProtoFEPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableMouseOverEvents = true;

	CameraController = CreateDefaultSubobject<UCameraControllerComponent>(TEXT("Camera Controller"));
	Pathfinder = CreateDefaultSubobject<UPathfinder>(TEXT("Pathfinder"));
	HighlightComponent = CreateDefaultSubobject<UHighlightComponent>(TEXT("Highlight Component"));
}



void AProtoFEPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	HighlightTile();
	// pathfinding - should I put this in another place?
	if (ShouldPathfind()) 
	{
		HighlightComponent->ResetPath();

		Path = Pathfinder->FindPathToTarget(
			GetSelectedCharacter()->MovementArea, 
			GetSelectedCharacter()->GridOccupyComponent->OccupiedTile, 
			SelectedTile
		);
		HighlightComponent->HighlightPath();
	}
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

	// gameplay functions
	InputComponent->BindAction("Select", IE_Pressed, this, &AProtoFEPlayerController::OnLeftClick);
	InputComponent->BindAction("Command", IE_Pressed, this, &AProtoFEPlayerController::OnRightClick);
	InputComponent->BindAction("Undo", IE_Pressed, this, &AProtoFEPlayerController::Undo);
}

void AProtoFEPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridManager::StaticClass(), Temp);
	check(Temp.Num() == 1);
	GridManager = Cast<AGridManager>(Temp[0]);
}

void AProtoFEPlayerController::HighlightTile() 
{
	PreviousTile = SelectedTile;

	if (SelectedTile)
		HighlightComponent->RemoveTileHighlight(SelectedTile);

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit)) // if hit tile
	{
		ATileActor* Tile = Cast<ATileActor>(Hit.GetActor());
		SelectedTile = GridManager->GetTile(Tile->GetActorLocation());
		HighlightComponent->HighlightSelectedTile();
	}
	else SelectedTile = nullptr;
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

void AProtoFEPlayerController::OnLeftClick() 
{
	if (GetSelectedCharacter() && GetSelectedCharacter()->ShouldUnSelect())
	{
		GetSelectedCharacter()->UnSelect();
	}
	
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit)) // if click hit something
	{
		if (ISelectable* ActorClicked = Cast<ISelectable>(Hit.GetActor())) // if click hit something that can be focused
			ActorClicked->HandleSelection();
	}
}

void AProtoFEPlayerController::OnRightClick() 
{
	if (GetSelectedCharacter())
	{
		if (ICommandable* SelectedAsCommandable = Cast<ICommandable>(GetSelectedCharacter()))
		{
			SelectedAsCommandable->ExecuteCommand();
		}
		if (GetSelectedCharacter()->ShouldUnSelect())
			GetSelectedCharacter()->UnSelect();
	}
}

void AProtoFEPlayerController::Undo() 
{
	RemoveHighlightedTiles();
}

void AProtoFEPlayerController::FocusCharacter(APlayerCharacter* Char)
{
	CameraController->FocusLocation(Char->GetActorLocation());
}

void AProtoFEPlayerController::AddHighlightedTiles(AEnemyCharacter* Char) 
{
	// set red highlight in character range
	for (UTile* Tile : Char->MovementArea)
	{
		Tile->Data.TileActor->SetAsEnemyRange();
	}
	
	EnemyRange.Characters.Add(Char);
	EnemyRange.Tiles.Append(Char->MovementArea);
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
		EnemyRange.Tiles.Append(C->MovementArea);
		for (UTile* Tile : C->MovementArea)
		{
			Tile->Data.TileActor->SetAsEnemyRange();
		}
	}
}

void AProtoFEPlayerController::RemoveHighlightedTiles() // TODO change name to RemoveEnemyRangeTiles()
{
	ResetEnemyTiles(EnemyRange.Tiles);

	for (AEnemyCharacter* Char : EnemyRange.Characters)
		Char->IsSelected = false;
	EnemyRange.Characters.Empty();
}

UTile* AProtoFEPlayerController::GetSelectedTile() 
{
	return SelectedTile;
}

void AProtoFEPlayerController::ResetEnemyTiles(TArray<UTile*> Tiles) 
{
	for (UTile* Tile : Tiles)
	{
		Tile->Data.TileActor->ResetAsEnemyRange();
		EnemyRange.Tiles.Remove(Tile);
	}
}

bool AProtoFEPlayerController::ShouldPathfind()
{
	return GetSelectedCharacter() 
	&& SelectedTile // cursor is on the grid
	&& PreviousTile != SelectedTile // cursor moved to a new tile
	&& (GetSelectedCharacter()->MovementArea.Contains(SelectedTile)  // selected tile is in character's movement range or occupied tile
		|| SelectedTile == GetSelectedCharacter()->GridOccupyComponent->OccupiedTile);
}

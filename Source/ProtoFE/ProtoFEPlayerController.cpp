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
#include "ConstructorHelpers.h"

AProtoFEPlayerController::AProtoFEPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableMouseOverEvents = true;

	CameraController = CreateDefaultSubobject<UCameraControllerComponent>(TEXT("Camera Controller"));
	Pathfinder = CreateDefaultSubobject<UPathfinder>(TEXT("Pathfinder"));
	HighlightComponent = CreateDefaultSubobject<UHighlightComponent>(TEXT("Highlight Component"));

	ConstructorHelpers::FClassFinder<UUserWidget> EscMenu(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/Widgets/EscapeMenu'"));
	if (EscMenu.Succeeded())
		EscapeMenuClass = EscMenu.Class;
}



void AProtoFEPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	HighlightTile();
	// pathfinding - should I put this in another place?
	if (ShouldPathfind()) 
	{
		HighlightComponent->ResetPath();

		APlayerCharacter* SelectedChar = Cast<APlayerCharacter>(GetSelectedActor().GetObject()); // we know with the "if (ShouldPathfind())" that the selected actor is a player character
		Path = Pathfinder->FindPathToTarget(
			SelectedChar->MovementArea, 
			SelectedChar->GridOccupyComponent->OccupiedTile, 
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
	InputComponent->BindAction("Back", IE_Pressed, this, &AProtoFEPlayerController::UnSelectEnemies);
	InputComponent->BindAction("Back", IE_Released, CameraController, &UCameraControllerComponent::SetNormalSpeed);
	InputComponent->BindAxis("RotateCamera", this, &AProtoFEPlayerController::HandleCameraRotate);

	// gameplay functions
	InputComponent->BindAction("Select", IE_Pressed, this, &AProtoFEPlayerController::OnLeftClick);
	InputComponent->BindAction("Command", IE_Pressed, this, &AProtoFEPlayerController::OnRightClick);
	InputComponent->BindAction("Undo", IE_Pressed, this, &AProtoFEPlayerController::ToggleEscapeMenu);
	InputComponent->BindAction("Alt", IE_Pressed, this, &AProtoFEPlayerController::SetAltMode);
	InputComponent->BindAction("Alt", IE_Released, this, &AProtoFEPlayerController::SetNormalMode);
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

TScriptInterface<ISelectable> AProtoFEPlayerController::GetSelectedActor()
{
	return SelectedActor;
}

void AProtoFEPlayerController::SetSelectedActor(TScriptInterface<ISelectable> Actor)
{
	SelectedActor = Actor;
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
	if (APlayerCharacter* SelectedChar = Cast<APlayerCharacter>(GetSelectedActor().GetObject()))
	{
		return SelectedChar // selected actor is a player character 
		&& SelectedTile // cursor is on the grid
		&& PreviousTile != SelectedTile // cursor moved to a new tile
		&& (SelectedChar->MovementArea.Contains(SelectedTile)  // selected tile is in character's movement range or occupied tile
			|| SelectedTile == SelectedChar->GridOccupyComponent->OccupiedTile);
	}
	return false;
}

void AProtoFEPlayerController::OnLeftClick() 
{
	if (GetSelectedActor() && GetSelectedActor()->ShouldUnSelect())
	{
		GetSelectedActor()->UnSelect();
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
	if (GetSelectedActor())
	{
		if (ICommandable* SelectedAsCommandable = Cast<ICommandable>(GetSelectedActor().GetObject()))
		{
			SelectedAsCommandable->ExecuteCommand();
		}
		if (GetSelectedActor()->ShouldUnSelect())
			GetSelectedActor()->UnSelect();
	}
}

void AProtoFEPlayerController::ToggleEscapeMenu() 
{
	if (!EscapeMenuObj) // hasn't been created yet
	{
		EscapeMenuObj = DisplayWidget(EscapeMenuClass);
		return;
	}
	if (EscapeMenuObj->IsInViewport()) // is showing
	{
		RemoveWidget(EscapeMenuObj);
	}
	else // has been created but isn't showing on screen
	{
		EscapeMenuObj = DisplayWidget(EscapeMenuClass);
	}
	
}

void AProtoFEPlayerController::UnSelectEnemies() 
{
	RemoveHighlightedTiles();
}

void AProtoFEPlayerController::HandleCameraRotate(float Value) 
{
	if (IsInputKeyDown(EKeys::MiddleMouseButton))
	{
		CameraController->RotateCamera(Value);
	}
}

void AProtoFEPlayerController::SetAltMode()
{
	DisplayMode = EDisplayMode::Detailed;
}

void AProtoFEPlayerController::SetNormalMode()
{
	DisplayMode = EDisplayMode::Normal;
}


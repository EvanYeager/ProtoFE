// ReSharper disable All
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
#include "Abilities/Ability.h"
#include "Interfaces/Focusable.h"
#include "Components/AbilityComponent.h"

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

	ConstructorHelpers::FClassFinder<UUserWidget> CombatUI(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/Widgets/CombatUI'"));
	if (CombatUI.Succeeded())
		CombatUIClass = CombatUI.Class;
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
		UTile* TargetTile = GetTargetTile();
		Path = Pathfinder->FindPathToTarget(
			SelectedChar->MovementArea, 
			SelectedChar->GridOccupyComponent->OccupiedTile, 
			TargetTile
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
	InputComponent->BindAction("Debug", IE_Pressed, this, &AProtoFEPlayerController::Debug);
	
}

void AProtoFEPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridManager::StaticClass(), Temp);
	check(Temp.Num() == 1);
	GridManager = Cast<AGridManager>(Temp[0]);

	CombatUIObj = DisplayWidget(CombatUIClass);
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
		&& (SelectedTile // cursor is on the grid
			&& PreviousTile != SelectedTile // cursor moved to a new tile
			&& (SelectedChar->MovementArea.Contains(SelectedTile)  // selected tile is in character's movement range or occupied tile
				|| SelectedTile == SelectedChar->GridOccupyComponent->OccupiedTile))
		|| Cast<AEnemyCharacter>(GetActorUnderCursor()); // cursor is on enemy
	}
	return false;
}

UTile* AProtoFEPlayerController::GetTargetTile()
{
	if (!GetSelectedActor()) goto QUICKRETURN;
	if (APlayerCharacter* PlayerSelected = Cast<APlayerCharacter>(GetSelectedActor().GetObject()))
	{
		if (!GetActorUnderCursor()) goto QUICKRETURN;
		if (AEnemyCharacter* EnemyUnderCursor = Cast<AEnemyCharacter>(GetActorUnderCursor()))
		{
			// get tiles next to enemy
			TArray<UTile*> AdjacentTiles = Pathfinder->GetTileNeighbors(EnemyUnderCursor->GridOccupyComponent->OccupiedTile); 
			
			// find closest adjacent tile to character
			UTile* ClosestTile = AdjacentTiles[0];
			int ClosestTileCost = 999;
			for (UTile* Tile : AdjacentTiles)
			{
				TArray<UTile*> Path = Pathfinder->FindPathToTarget(PlayerSelected->MovementArea, 
					PlayerSelected->GridOccupyComponent->OccupiedTile, 
					Tile
				);
				if (Path.Num() == 0) continue;
				int TileCost = Path[Path.Num() - 1]->FinalCost - 1;
				if (TileCost < ClosestTileCost)
				{
					ClosestTileCost = TileCost;
					ClosestTile = Tile;
				}
			}
	
			return ClosestTile;
		}
	}
	QUICKRETURN: return SelectedTile;
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
		if (ISelectable* ActorClicked = Cast<ISelectable>(Hit.GetActor())) // if click hit something that can be selected
			ActorClicked->HandleSelection();


		if (IFocusable* FocusableActor = Cast<IFocusable>(Hit.GetActor())) // if click hit something that can be focused by the camera
		{
			if (ClickedRecently) // if this click is a double click
			{
				FocusableActor->Focus();
			}
			else
			{
				ClickedRecently = true;
				GetWorld()->GetTimerManager().SetTimer(DoubleClickTimer, 
					this, 
					&AProtoFEPlayerController::DoubleClickReset, 
					DoubleClickDelay, 
					false
				);
			}
		}
	}
}

void AProtoFEPlayerController::OnRightClick() 
{
	if (GetSelectedActor())
	{
		AActor* ActorUnderCursor = GetActorUnderCursor();
		if (ActorUnderCursor && Cast<AEnemyCharacter>(ActorUnderCursor))
		{
			// move next to enemy and attack 
		}
		if (ICommandable* SelectedAsCommandable = Cast<ICommandable>(GetSelectedActor().GetObject()))
			SelectedAsCommandable->ExecuteCommand();
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

void AProtoFEPlayerController::DoubleClickReset() 
{
	ClickedRecently = false;
}

AActor* AProtoFEPlayerController::GetActorUnderCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	return Hit.GetActor();
}

void AProtoFEPlayerController::Debug()
{
	if (SelectedActor)
	{
		UAbility* Ability = *Cast<AProtoFECharacter>(SelectedActor.GetObject())->AbilityComponent->GetAbilityHotbar().Find(1);
		if (!Ability) 
		{
			UE_LOG(LogTemp, Warning, TEXT("no ablility on unit"));
			return;
		}
		Ability->Activate();
	}
} 

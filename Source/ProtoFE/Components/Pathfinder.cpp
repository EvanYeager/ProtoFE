#include "Pathfinder.h"
#include "ProtoFECharacter.h"
// #include "Actors/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Actors/TerrainModifiers/TerrainMod.h"
#include "Components/GridOccupyComponent.h"

// Sets default values for this component's properties
UPathfinder::UPathfinder()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPathfinder::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(Owner);
}

bool UPathfinder::IsTileOccupied(UTile* Tile)
{
	return IsValid(Tile->Data.OccupiedBy);
}

bool UPathfinder::IsCurrentTileOccupiedByPlayer()
{
	return IsTileOccupied(CurrentTile) && CurrentTile->Data.OccupiedBy->Information.Team == ETeam::Player; // IsTileOccupied() is there to protect from nullptrs
}

bool UPathfinder::IsCurrentTileOccupiedByEnemy()
{
	return IsTileOccupied(CurrentTile) && CurrentTile->Data.OccupiedBy->Information.Team == ETeam::Enemy; // IsTileOccupied() is there to protect from nullptrs
}

TArray<UTile*> UPathfinder::GetTileNeighbors(UTile* Tile, bool IncludePlayers, bool IncludeEnemies)
{
	TArray<UTile*> OutNeighbors;
	if (!Tile) return OutNeighbors;
	TArray<FIntPoint> CardinalDirections = 
	{
		FIntPoint(0, 1),  // Up
		FIntPoint(1, 0),	// Right
		FIntPoint(0, -1),	// Down
		FIntPoint(-1, 0)	// Left
	};

	for (FIntPoint Direction : CardinalDirections)
	{
		FIntPoint NeighborCoords = Tile->Data.Coordinates + Direction;
		UTile* AdjacentTile = AGridManager::GetTile(NeighborCoords, Grid);
		if (!AdjacentTile) continue;
		if (MovementArea.Num() > 0 && !MovementArea.Contains(AdjacentTile)) continue; // if finding path to target and neighbor is not in movement options, don't consider it

		if (IsTileOccupied(AdjacentTile))
		{
			AProtoFECharacter* Char = AdjacentTile->Data.OccupiedBy;
			if (!IncludePlayers ^ (Char->Information.Team == ETeam::Player) || IncludeEnemies ^ (Char->Information.Team == ETeam::Player)) // Returns true if both booleans are the same, i.e. char is a player and include players, or if an enemy and include enemies
			{
				OutNeighbors.AddUnique(AdjacentTile);
			}
		}
		else
		{
			OutNeighbors.AddUnique(AdjacentTile);
		}
	}
	return OutNeighbors;
}


TArray<UTile*> UPathfinder::BreadthSearch(AProtoFECharacter* CharacterCalling, TArray<UTile*>& TilesToMakeRed, TArray<AProtoFECharacter*>& CharsInRange)
{
	ClearMemberVariables();
	CharacterPathfinding = CharacterCalling;
	MaxMovement = CharacterCalling->Information.Movement;
	NumOfPossibleTiles = GetMovementOptionArea(MaxMovement + 1); // The + 1 is so it will search for the red highlights
	IsPlayerPathfinding = CharacterCalling->Information.Team == ETeam::Player;
	UTile* StartTile = CharacterCalling->GridOccupyComponent->OccupiedTile;
	Queue.Add(StartTile);
	ValidTiles.Add(StartTile);
	Grid = *AGridManager::GetGrid(GetWorld());
	ResetFinalCosts();

	for (int32 i = 0; i < NumOfPossibleTiles; i++)
	{
		if (Queue.Num() == 0) break;
		for (UTile* NeighborTile : GetTileNeighbors(Queue[0]))
		{
			CurrentTile = NeighborTile;
			EvaluateCurrentTileForBreadthSearch();
		}
		Queue.RemoveAt(0);
	}

	FilterRedTiles();
	TilesToMakeRed = RedTiles;
	CharsInRange = CharsFound;
	return ValidTiles;
}

void UPathfinder::ClearMemberVariables()
{
	Queue.Empty();
	ValidTiles.Empty();
	RedTiles.Empty();
	CharsFound.Empty();
	Path.Empty();
	Queue.Empty();
	EstablishedTiles.Empty();
	MovementArea.Empty();
	FinalCostFromCurrentNeighbor = 0;
}

int32 UPathfinder::GetMovementOptionArea(int32 Movement)
{
	return ((Movement * 2) + 2) * (Movement + 1) - ((Movement * 2) + 1);
}

void UPathfinder::ResetFinalCosts()
{
	for(FGridColumn Col : Grid)
	{
		for (UTile* Tile : Col.Tiles)
		{
			Tile->FinalCost = 0;
		}
	}
}

void UPathfinder::EvaluateCurrentTileForBreadthSearch()
{
	MoveCost = Queue[0]->FinalCost;
	MoveCost += *CharacterPathfinding->TerrainMoveCost.Find(CurrentTile->Data.Terrain); // add terrain move cost for character
	if (TileShouldBeRed())
	{
		RedTiles.AddUnique(CurrentTile);
		if (EnemyIsPathfindingAndCurrentTileIsOccupiedByPlayer())
		{
			CharsFound.AddUnique(CurrentTile->Data.OccupiedBy);
		}
	}
	else if (CurrentTileIsWalkable())
	{
		AddCurrentTileToValidTiles();
	}
}

bool UPathfinder::TileShouldBeRed()
{
	return MoveCost > MaxMovement || // Tile outside movement range
	CurrentTile->Data.Terrain == ETerrain::Impossible ||
	PlayerIsPathfindingAndCurrentTileIsOccupiedByEnemy() ||
	EnemyIsPathfindingAndCurrentTileIsOccupiedByPlayer();
}

bool UPathfinder::PlayerIsPathfindingAndCurrentTileIsOccupiedByEnemy()
{
	return IsPlayerPathfinding && IsCurrentTileOccupiedByEnemy();
}

bool UPathfinder::EnemyIsPathfindingAndCurrentTileIsOccupiedByPlayer()
{
	return !IsPlayerPathfinding && IsCurrentTileOccupiedByPlayer();
}

void UPathfinder::AddCurrentTileToValidTiles()
{
	Queue.AddUnique(CurrentTile);
	CurrentTile->FinalCost = MoveCost;
	ValidTiles.AddUnique(CurrentTile);
}

bool UPathfinder::CurrentTileIsWalkable()
{
	return MoveCost <= MaxMovement && (!ValidTiles.Contains(CurrentTile) || 
	MoveCost < CurrentTile->FinalCost);
}

void UPathfinder::FilterRedTiles() 
{
	TArray<UTile*> Filtered;
	// find red tiles to filter
	for (UTile* RedTile : RedTiles)
	{
		if (ValidTiles.Contains(RedTile))
		{
			Filtered.Add(RedTile);
		}
	}
	// remove them
	for (UTile* ToRemove : Filtered)
	{
		RedTiles.Remove(ToRemove);
	}
}

/* ==========================================
	Breadth Search ends and Find Path starts	
========================================== */


TArray<UTile*> UPathfinder::FindPathToTarget(TArray<UTile*> AvailableMovementArea, UTile* StartTile, UTile* TargetTile, bool IsPlayerCalling)
{
	Path.Empty();
	if (!StartTile || !TargetTile) return Path;
	if (StartTile == TargetTile)
	{
		Path.Add(StartTile);
		return Path;
	}
	
	ClearMemberVariables();
	ResetPathfindingTileVars();
	MovementArea = AvailableMovementArea;
	InitialTile = StartTile;
	CurrentTile = InitialTile;
	DestinationTile = TargetTile;
	IsPlayerPathfinding = IsPlayerCalling;
	int32 EstimatedCost = GetEstimatedCostToTile(DestinationTile);
	CurrentTile->FinalCost = EstimatedCost;
	CurrentTile->EstimatedCostToTarget = EstimatedCost;
	Queue.AddUnique(InitialTile);

	Pathfind();
	
	MovementArea.Empty(); // I empty this so it's not effecting GetTileNeighbors function
	return Path;
}

void UPathfinder::ResetPathfindingTileVars()
{
	for (FGridColumn Col : Grid)
	{
		for (UTile* Tile : Col.Tiles)
		{
			Tile->FinalCost = 0;
			Tile->CostFromStart = 0;
			Tile->EstimatedCostToTarget = 0;
			Tile->PreviousTile = nullptr;
		}
	}
}

void UPathfinder::Pathfind()
{
	while (Queue.Num() > 0)
	{
		CurrentTile = FindCheapestInQueue();
		Queue.Remove(CurrentTile);
		EstablishedTiles.AddUnique(CurrentTile);
		TArray<UTile*> Neighbors = GetTileNeighbors(CurrentTile, IsPlayerPathfinding, !IsPlayerPathfinding);
		for (UTile* CurrentNeighbor : Neighbors)
		{
			if (!EstablishedTiles.Contains(CurrentNeighbor) ||
			FinalCostFromCurrentNeighbor < CurrentNeighbor->CostFromStart)
			{
				FinalCostFromCurrentNeighbor = CurrentTile->CostFromStart + *CharacterPathfinding->TerrainMoveCost.Find(CurrentNeighbor->Data.Terrain);
				SetTileVariables(CurrentTile, CurrentNeighbor);
				if (CurrentNeighbor == DestinationTile)
				{
					Path = RetracePath();
					return;
				}
				Queue.AddUnique(CurrentNeighbor);
			}
		}
	}
}

int32 UPathfinder::GetEstimatedCostToTile(UTile* Tile)
{
	FIntPoint TempDistance = Tile->Data.Coordinates - DestinationTile->Data.Coordinates;
	FIntPoint FinalDistance = FIntPoint(abs(TempDistance.X), abs(TempDistance.Y));
	return int32(FinalDistance.X + FinalDistance.Y);
}

UTile* UPathfinder::FindCheapestInQueue()
{
	UTile* QueueCheapest = Queue[0];
	for (UTile* Element : Queue)
	{
		if (QueueElementIsBetterThanQueueCheapest(Element, QueueCheapest))
		{
			QueueCheapest = Element;
		}
	}
	return QueueCheapest;
}

bool UPathfinder::QueueElementIsBetterThanQueueCheapest(UTile* QueueElem, UTile* QueueCheapest)
{
	// Conditions for being true: QueueElem FinalCost is less than QueueCheapest's, or if they are equal but QueueElem is closer to destination
	return (QueueElem->FinalCost < QueueCheapest->FinalCost) || 
	(QueueElem->FinalCost == QueueCheapest->FinalCost && 
	QueueElem->EstimatedCostToTarget < QueueCheapest->EstimatedCostToTarget);
}

void UPathfinder::SetTileVariables(UTile* PreviousTile, UTile* CurrentNeighbor)
{
	CurrentNeighbor->FinalCost = FinalCostFromCurrentNeighbor + *CharacterPathfinding->TerrainMoveCost.Find(CurrentNeighbor->Data.Terrain);
	CurrentNeighbor->CostFromStart = FinalCostFromCurrentNeighbor;
	CurrentNeighbor->EstimatedCostToTarget = GetEstimatedCostToTile(CurrentNeighbor);
	CurrentNeighbor->PreviousTile = PreviousTile;
}

TArray<UTile*> UPathfinder::RetracePath()
{
	TArray<UTile*> InvertedPath;
	TArray<UTile*> OutPath;
	CurrentTile = DestinationTile;

	while (CurrentTile != InitialTile)
	{
		InvertedPath.AddUnique(CurrentTile);
		CurrentTile = CurrentTile->PreviousTile;
	}
	for (int32 i = InvertedPath.Num() - 1; i >= 0; i--)
	{
		OutPath.AddUnique(InvertedPath[i]);
	}
	return OutPath;
}


TArray<UTile*> UPathfinder::FindPathAsEnemy(UTile* StartTile, TArray<UTile*> PossibleTargetTiles)
{
	TArray<UTile*> CurrentPath,
	BestPath;
	int32 CurrentPathCost,
	BestPathCost = 999;
	for(UTile* TargetTileElement : PossibleTargetTiles)
	{
		TArray<UTile*> Argument;
		CurrentPath = FindPathToTarget(Argument, StartTile, TargetTileElement, false);
		CurrentPathCost = CurrentPath.Last()->FinalCost; // Make sure to keep the pathfinding variables non-zero after pathfinding so I can use this outside the function
		if (IsThisFirstEnemyPathfind(BestPath) || CurrentPathCost < BestPathCost)
		{
			BestPath = CurrentPath;
			BestPathCost = BestPath.Last()->FinalCost;
		}
	}
	return BestPath;
}

bool UPathfinder::IsThisFirstEnemyPathfind(TArray<UTile*> BestPath)
{
	return !BestPath.IsValidIndex(0);
}


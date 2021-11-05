#include "Pathfinder.h"
#include "ProtoFECharacter.h"
#include "Actors/GridManager.h"
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

FGridData* UPathfinder::GetTileStruct(FIntPoint Tile)
{
	return GridManager->Grid.Find(Tile);
}

bool UPathfinder::IsTileOccupied(FIntPoint Tile)
{
	return IsValid(GetTileStruct(Tile)->OccupiedBy);
}

bool UPathfinder::IsCurrentTileOccupiedByPlayer()
{
	return IsTileOccupied(CurrentTile) && GetTileStruct(CurrentTile)->OccupiedBy->Information.Team == ETeam::Player; // IsTileOccupied() is there to protect from nullptrs
}

bool UPathfinder::IsCurrentTileOccupiedByEnemy()
{
	return IsTileOccupied(CurrentTile) && GetTileStruct(CurrentTile)->OccupiedBy->Information.Team == ETeam::Enemy; // IsTileOccupied() is there to protect from nullptrs
}

TArray<FIntPoint> UPathfinder::GetTileNeighbors(FIntPoint Tile, bool IncludePlayers, bool IncludeEnemies)
{
	TArray<FIntPoint> OutNeighbors;
	TArray<FIntPoint> CardinalDirections = 
	{
		FIntPoint(0, 1),  // Up
		FIntPoint(1, 0),	// Right
		FIntPoint(0, -1),	// Down
		FIntPoint(-1, 0)	// Left
	};

	for (FIntPoint Direction : CardinalDirections)
	{
		FIntPoint AdjacentTile = Tile + Direction;

		if(MovementArea.Num() > 0) // This checks if the MovementArea array has data. It only should when this is called in the FindPathToTarget function
		{
			if (MovementArea.Contains(AdjacentTile))
			{
				goto EVALUATE;
			}
		}
		else if (GetTileStruct(AdjacentTile))
		{
			EVALUATE:if (IsTileOccupied(AdjacentTile))
			{
				FGridData* AdjacentTileStruct = GetTileStruct(AdjacentTile);
				AProtoFECharacter* Char = AdjacentTileStruct->OccupiedBy;
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
	}
	return OutNeighbors;
}


TArray<FIntPoint> UPathfinder::BreadthSearch(AProtoFECharacter* CharacterCalling, TArray<FIntPoint>& TilesToMakeRed, TArray<AProtoFECharacter*>& CharsInRange)
{
	ClearMemberVariables();
	CharacterPathfinding = CharacterCalling;
	MaxMovement = CharacterCalling->Information.Movement;
	NumOfPossibleTiles = GetMovementOptionArea(MaxMovement + 1); // The + 1 is so it will search for the red highlights
	IsPlayerPathfinding = CharacterCalling->Information.Team == ETeam::Player;
	FIntPoint StartTile = CharacterCalling->GridOccupyComponent->OccupiedTile; 
	Queue.Add(StartTile);
	ValidTiles.Add(StartTile);
	GridManager = Owner->GridManager;
	ResetFinalCosts();

	for (int32 i = 0; i < NumOfPossibleTiles; i++)
	{
		if (Queue.Num() == 0) break;
		for (FIntPoint NeighborTile : GetTileNeighbors(Queue[0]))
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
	for(auto& Elem : GridManager->Grid)
	{
		GetTileStruct(Elem.Key)->FinalCost = 0;
	}
}

void UPathfinder::EvaluateCurrentTileForBreadthSearch()
{
	CurrentTileStruct = GetTileStruct(CurrentTile);
	MoveCost = GetTileStruct(Queue[0])->FinalCost;
	MoveCost += *CharacterPathfinding->TerrainMoveCost.Find(CurrentTileStruct->Terrain); // add terrain move cost for character
	if (TileShouldBeRed())
	{
		RedTiles.AddUnique(CurrentTile);
		if (EnemyIsPathfindingAndCurrentTileIsOccupiedByPlayer())
		{
			CharsFound.AddUnique(CurrentTileStruct->OccupiedBy);
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
	CurrentTileStruct->Terrain == ETerrain::Impossible ||
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
	CurrentTileStruct->FinalCost = MoveCost;
	ValidTiles.AddUnique(CurrentTile);
}

bool UPathfinder::CurrentTileIsWalkable()
{
	return MoveCost <= MaxMovement && (!ValidTiles.Contains(CurrentTile) || 
	MoveCost < CurrentTileStruct->FinalCost);
}

void UPathfinder::FilterRedTiles() 
{
	TArray<FIntPoint> Filtered;
	// find red tiles to filter
	for (FIntPoint RedTile : RedTiles)
	{
		if (ValidTiles.Contains(RedTile))
		{
			Filtered.Add(RedTile);
		}
	}
	// remove them
	for (FIntPoint ToRemove : Filtered)
	{
		RedTiles.Remove(ToRemove);
	}
}

/* ==========================================
	Breadth Search ends and Find Path starts	
========================================== */


TArray<FIntPoint> UPathfinder::FindPathToTarget(TArray<FIntPoint> AvailableMovementArea, FIntPoint StartTile, FIntPoint TargetTile, bool IsPlayerCalling)
{
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
	CurrentTileStruct = GetTileStruct(CurrentTile);
	CurrentTileStruct->FinalCost = EstimatedCost;
	CurrentTileStruct->EstimatedCostToTarget = EstimatedCost;
	Queue.AddUnique(InitialTile);

	Pathfind();
	
	MovementArea.Empty(); // I empty this so it's not effecting GetTileNeighbors function
	return Path;
}

void UPathfinder::ResetPathfindingTileVars()
{
	for (auto& Element : GridManager->Grid)
	{
		FGridData* TileStruct = GetTileStruct(Element.Key);
		TileStruct->FinalCost = 0;
		TileStruct->CostFromStart = 0;
		TileStruct->EstimatedCostToTarget = 0;
		TileStruct->PreviousTileCoords = FIntPoint(0, 0);
	}
}

void UPathfinder::Pathfind()
{
	while (Queue.Num() > 0)
	{
		CurrentTile = FindCheapestInQueue();
		Queue.Remove(CurrentTile);
		EstablishedTiles.AddUnique(CurrentTile);
		TArray<FIntPoint> Neighbors = GetTileNeighbors(CurrentTile, IsPlayerPathfinding, !IsPlayerPathfinding);
		for (FIntPoint CurrentNeighbor : Neighbors)
		{
			if (!EstablishedTiles.Contains(CurrentNeighbor) ||
			FinalCostFromCurrentNeighbor < GetTileStruct(CurrentNeighbor)->CostFromStart)
			{
				FinalCostFromCurrentNeighbor = GetTileStruct(CurrentTile)->CostFromStart + *CharacterPathfinding->TerrainMoveCost.Find(GetTileStruct(CurrentNeighbor)->Terrain);
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

int32 UPathfinder::GetEstimatedCostToTile(FIntPoint Tile)
{
	FIntPoint TempDistance = Tile - DestinationTile;
	FIntPoint FinalDistance = FIntPoint(abs(TempDistance.X), abs(TempDistance.Y));
	return int32(FinalDistance.X + FinalDistance.Y);
}

FIntPoint UPathfinder::FindCheapestInQueue()
{
	FIntPoint QueueCheapest = Queue[0];
	for (FIntPoint Element : Queue)
	{
		FGridData* QueueElemStruct = GetTileStruct(Element);
		FGridData* QueueCheapestStruct = GetTileStruct(QueueCheapest);
		if (QueueElementIsBetterThanQueueCheapest(Element, QueueCheapest))
		{
			QueueCheapest = Element;
		}
	}
	return QueueCheapest;
}

bool UPathfinder::QueueElementIsBetterThanQueueCheapest(FIntPoint QueueElem, FIntPoint QueueCheapest)
{
	FGridData* QueueElemStruct = GetTileStruct(QueueElem);
	FGridData* QueueCheapestStruct = GetTileStruct(QueueCheapest);
	// Conditions for being true: QueueElem FinalCost is less than QueueCheapest's, or if they are equal but QueueElem is closer to destination
	return (QueueElemStruct->FinalCost < QueueCheapestStruct->FinalCost) || 
	(QueueElemStruct->FinalCost == QueueCheapestStruct->FinalCost && 
	QueueElemStruct->EstimatedCostToTarget < QueueCheapestStruct->EstimatedCostToTarget);
}

void UPathfinder::SetTileVariables(FIntPoint PreviousTile, FIntPoint CurrentNeighbor)
{
	FGridData* CurrentNeighborStruct = GetTileStruct(CurrentNeighbor);
	CurrentNeighborStruct->FinalCost = FinalCostFromCurrentNeighbor + *CharacterPathfinding->TerrainMoveCost.Find(CurrentNeighborStruct->Terrain);
	CurrentNeighborStruct->CostFromStart = FinalCostFromCurrentNeighbor;
	CurrentNeighborStruct->EstimatedCostToTarget = GetEstimatedCostToTile(CurrentNeighbor);
	CurrentNeighborStruct->PreviousTileCoords = PreviousTile;
}

TArray<FIntPoint> UPathfinder::RetracePath()
{
	TArray<FIntPoint> InvertedPath;
	TArray<FIntPoint> OutPath;
	CurrentTile = DestinationTile;

	while (CurrentTile != InitialTile)
	{
		InvertedPath.AddUnique(CurrentTile);
		CurrentTile = GetTileStruct(CurrentTile)->PreviousTileCoords;
	}
	for (int32 i = InvertedPath.Num() - 1; i >= 0; i--)
	{
		OutPath.AddUnique(InvertedPath[i]);
	}
	return OutPath;
}


TArray<FIntPoint> UPathfinder::FindPathAsEnemy(FIntPoint StartTile, TArray<FIntPoint> PossibleTargetTiles)
{
	TArray<FIntPoint> CurrentPath,
	BestPath;
	int32 CurrentPathCost,
	BestPathCost = 999;
	for(FIntPoint TargetTileElement : PossibleTargetTiles)
	{
		TArray<FIntPoint> Argument;
		CurrentPath = FindPathToTarget(Argument, StartTile, TargetTileElement, false);
		CurrentPathCost = GetTileStruct(CurrentPath.Last())->FinalCost; // Make sure to keep the pathfinding variables non-zero after pathfinding so I can use this outside the function
		if (IsThisFirstEnemyPathfind(BestPath))
		{
			BestPath = CurrentPath;
			BestPathCost = GetTileStruct(BestPath.Last())->FinalCost;
		}
		else if (CurrentPathCost < BestPathCost)
		{
			BestPath = CurrentPath;
			BestPathCost = GetTileStruct(BestPath.Last())->FinalCost;
		}
	}
	return BestPath;
}

bool UPathfinder::IsThisFirstEnemyPathfind(TArray<FIntPoint> BestPath)
{
	return !BestPath.IsValidIndex(0);
}


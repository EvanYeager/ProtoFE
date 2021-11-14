#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/GridManager.h"
#include "Pathfinder.generated.h"

class AProtoFECharacter;
// struct FGridRow;
class AGridManager;
class AProtoFEPlayerController;
class UTile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOFE_API UPathfinder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfinder();


	UPROPERTY(EditAnywhere)
	AProtoFEPlayerController* Owner;

	bool IsTileOccupied(UTile* Tile);

	bool IsCurrentTileOccupiedByPlayer();

	UFUNCTION(BlueprintCallable, BluePrintPure, Category = "Utility")
	bool IsCurrentTileOccupiedByEnemy();

	UFUNCTION(BlueprintCallable, BluePrintPure)
	// This overload is for general use
	TArray<UTile*> GetTileNeighbors(UTile* Tile, bool IncludePlayers = true, bool IncludeEnemies = true);

	UFUNCTION(BlueprintCallable, Category = "Grid Struct")
	void ResetPathfindingTileVars();

	UFUNCTION(BlueprintCallable)
	// Returns: Tiles outside movement range to make red if player is calling, Chars found if enemy is calling, and return value is the valid movement area.
	TArray<UTile*> BreadthSearch(AProtoFECharacter* CharacterCalling, TArray<UTile*>& TilesToMakeRed, TArray<AProtoFECharacter*>& CharsInRange);

	TArray<UTile*> RetracePath();

	UFUNCTION(BlueprintCallable)
	// IsPlayerCalling is true if a player character is pathfinding, false if an enemy is. This way if an enemy calls this they can go through other enemies.
	TArray<UTile*> FindPathToTarget(TArray<UTile*> AvailableMovementArea, UTile* StartTile, UTile* TargetTile, bool IsPlayerCalling = true); 

	UFUNCTION(BlueprintCallable)
	TArray<UTile*> FindPathAsEnemy(UTile* StartTile, TArray<UTile*> PossibleTargetTiles);

protected:
	virtual void BeginPlay() override;

private:
	// BreadthSearch
	int32 GetMovementOptionArea(int32 Movement);
	void ResetFinalCosts();
	// Evaluates tile and adds it to appropriate arrays like ValidTiles, TilesToMakeRed, and also populates the CharsInRange array.
	void EvaluateCurrentTileForBreadthSearch();
	bool TileShouldBeRed();
	bool EnemyIsPathfindingAndCurrentTileIsOccupiedByPlayer();
	bool PlayerIsPathfindingAndCurrentTileIsOccupiedByEnemy();
	void AddCurrentTileToValidTiles();
	bool CurrentTileIsWalkable();
	// Some of the tiles that clear the check to go into the RedTiles array are also in the ValidTiles, and this function removes those.
	void FilterRedTiles();

	// FindPathToTarget
	void Pathfind();
	int32 GetEstimatedCostToTile(UTile* Tile);
	UTile* FindCheapestInQueue();
	bool QueueElementIsBetterThanQueueCheapest(UTile* QueueElem, UTile* QueueCheapest);
	void SetTileVariables(UTile* PreviousTile, UTile* CurrentNeighbor);
	bool IsThisFirstEnemyPathfind(TArray<UTile*> BestPath);

	// Both
	void ClearMemberVariables();

	// BreadthSearch
	TArray<AProtoFECharacter*> CharsFound;
	int32 NumOfPossibleTiles;

	// FindPathToTarget
	TArray<UTile*> MovementArea;
	UTile* InitialTile;
	UTile* DestinationTile;
	TArray<UTile*> Path;
	TArray<UTile*> EstablishedTiles;
	int32 FinalCostFromCurrentNeighbor;

	// BreadthSearch and FindPathToTarget
	AProtoFECharacter* CharacterPathfinding;
	TArray<UTile*> Queue;
	UTile* CurrentTile;
	TArray<UTile*> ValidTiles;
	int32 MaxMovement;
	int32 MoveCost;
	TArray<UTile*> RedTiles;
	bool IsPlayerPathfinding;
		
	TArray<FGridRow> Grid;
};

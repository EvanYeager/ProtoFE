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

	UFUNCTION(BlueprintCallable, BluePrintPure, Category = Utility)
	// This overload is for general use
	TArray<UTile*> GetTileNeighbors(UTile* Tile, bool IncludePlayers = true, bool IncludeEnemies = true);

	UFUNCTION(BlueprintCallable)
	// Returns: Tiles outside movement range to make red if player is calling, Chars found if enemy is calling, and return value is the valid movement area.
	TArray<UTile*> BreadthSearch(AProtoFECharacter* CharacterCalling, TArray<UTile*>& TilesToMakeRed, TArray<AProtoFECharacter*>& CharsInRange);

	UFUNCTION(BlueprintCallable)
	// IsPlayerCalling is true if a player character is pathfinding, false if an enemy is. This way if an enemy calls this they can go through other enemies.
	TArray<UTile*> FindPathToTarget(TArray<UTile*> AvailableMovementArea, UTile* StartTile, UTile* TargetTile, bool IsPlayerCalling = true); 

	UFUNCTION(BlueprintCallable)
	TArray<UTile*> FindPathAsEnemy(UTile* StartTile, TArray<UTile*> PossibleTargetTiles);

protected:
	virtual void BeginPlay() override;

private:
	
	/**
	 *  BreadthSearch functions
	*/

	UFUNCTION()
	int32 GetMovementOptionArea(int32 Movement);
	UFUNCTION()
	void ResetFinalCosts();
	UFUNCTION()
	bool IsCurrentTileOccupiedByPlayer();
	UFUNCTION()
	bool IsCurrentTileOccupiedByEnemy();
	UFUNCTION()
	// Evaluates tile and adds it to appropriate arrays like ValidTiles, TilesToMakeRed, and also populates the CharsInRange array.
	void EvaluateCurrentTileForBreadthSearch();
	UFUNCTION()
	bool TileShouldBeRed();
	UFUNCTION()
	bool EnemyIsPathfindingAndCurrentTileIsOccupiedByPlayer();
	UFUNCTION()
	bool PlayerIsPathfindingAndCurrentTileIsOccupiedByEnemy();
	UFUNCTION()
	void AddCurrentTileToValidTiles();
	UFUNCTION()
	bool CurrentTileIsWalkable();
	UFUNCTION()
	// Some of the tiles that clear the check to go into the RedTiles array are also in the ValidTiles, and this function removes those.
	void FilterRedTiles();

	/** 
	 * Both
	*/
	
	UFUNCTION()
	void ResetPathfindingTileVars();
	UFUNCTION()
	bool IsTileOccupied(UTile* Tile);
	UFUNCTION()
	void ClearMemberVariables();
	
	/**
	 *  FindPathToTarget functions
	*/

	UFUNCTION()
	void Pathfind();
	UFUNCTION()
	int32 GetEstimatedCostToTile(UTile* Tile);
	UFUNCTION()
	UTile* FindCheapestInQueue();
	UFUNCTION()
	bool QueueElementIsBetterThanQueueCheapest(UTile* QueueElem, UTile* QueueCheapest);
	UFUNCTION()
	void SetTileVariables(UTile* PreviousTile, UTile* CurrentNeighbor);
	UFUNCTION()
	bool IsThisFirstEnemyPathfind(TArray<UTile*> BestPath);
	UFUNCTION()
	TArray<UTile*> RetracePath();


	/**
	 *  BreadthSearch variables
	*/

	UPROPERTY()
	TArray<AProtoFECharacter*> CharsFound;
	UPROPERTY()
	int32 NumOfPossibleTiles;

	/**
	 *  BreadthSearch and FindPathToTarget variables
	*/
	UPROPERTY()
	TArray<FGridColumn> Grid;
	UPROPERTY()
	AProtoFECharacter* CharacterPathfinding;
	UPROPERTY()
	TArray<UTile*> Queue;
	UPROPERTY()
	UTile* CurrentTile;
	UPROPERTY()
	TArray<UTile*> ValidTiles;
	UPROPERTY()
	int32 MaxMovement;
	UPROPERTY()
	int32 MoveCost;
	UPROPERTY()
	TArray<UTile*> RedTiles;
	UPROPERTY()
	bool IsPlayerPathfinding;

	/**
	 *  FindPathToTarget variables
	*/
	UPROPERTY()
	TArray<UTile*> MovementArea;
	UPROPERTY()
	UTile* InitialTile;
	UPROPERTY()
	UTile* DestinationTile;
	UPROPERTY()
	TArray<UTile*> Path;
	UPROPERTY()
	TArray<UTile*> EstablishedTiles;
	UPROPERTY()
	int32 FinalCostFromCurrentNeighbor;

	UPROPERTY()
	AProtoFEPlayerController* Owner;
};

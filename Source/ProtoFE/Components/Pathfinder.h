#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pathfinder.generated.h"

class AProtoFECharacter;
struct FGridData;
class AGridManager;
class AProtoFEPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOFE_API UPathfinder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfinder();


	UPROPERTY(EditAnywhere)
	AProtoFEPlayerController* Owner;

	bool IsTileOccupied(FIntPoint Tile);

	bool IsCurrentTileOccupiedByPlayer();

	UFUNCTION(BlueprintCallable, BluePrintPure, Category = "Utility")
	bool IsCurrentTileOccupiedByEnemy();

	FGridData* GetTileStruct(FIntPoint Tile);

	UFUNCTION(BlueprintCallable, BluePrintPure)
	// This overload is for general use
	TArray<FIntPoint> GetTileNeighbors(FIntPoint Tile, bool IncludePlayers = true, bool IncludeEnemies = true);

	UFUNCTION(BlueprintCallable, Category = "Grid Struct")
	void ResetPathfindingTileVars();

	UFUNCTION(BlueprintCallable)
	// Returns: Tiles outside movement range to make red if player is calling, Chars found if enemy is calling, and return value is the valid movement area.
	TArray<FIntPoint> BreadthSearch(AProtoFECharacter* CharacterCalling, TArray<FIntPoint>& TilesToMakeRed, TArray<AProtoFECharacter*>& CharsInRange);

	TArray<FIntPoint> RetracePath();

	UFUNCTION(BlueprintCallable)
	// IsPlayerCalling is true if a player character is pathfinding, false if an enemy is. This way if an enemy calls this they can go through other enemies.
	TArray<FIntPoint> FindPathToTarget(TArray<FIntPoint> AvailableMovementArea, FIntPoint StartTile, FIntPoint TargetTile, bool IsPlayerCalling = true); 

	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> FindPathAsEnemy(FIntPoint StartTile, TArray<FIntPoint> PossibleTargetTiles);

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
	int32 GetEstimatedCostToTile(FIntPoint Tile);
	FIntPoint FindCheapestInQueue();
	bool QueueElementIsBetterThanQueueCheapest(FIntPoint QueueElem, FIntPoint QueueCheapest);
	void SetTileVariables(FIntPoint PreviousTile, FIntPoint CurrentNeighbor);
	bool IsThisFirstEnemyPathfind(TArray<FIntPoint> BestPath);

	// Both
	void ClearMemberVariables();

	// BreadthSearch
	TArray<AProtoFECharacter*> CharsFound;
	int32 NumOfPossibleTiles;

	// FindPathToTarget
	TArray<FIntPoint> MovementArea;
	FIntPoint InitialTile;
	FIntPoint DestinationTile;
	TArray<FIntPoint> Path;
	TArray<FIntPoint> EstablishedTiles;
	int32 FinalCostFromCurrentNeighbor;

	// BreadthSearch and FindPathToTarget
	AProtoFECharacter* CharacterPathfinding;
	TArray<FIntPoint> Queue;
	FIntPoint CurrentTile;
	FGridData* CurrentTileStruct;
	TArray<FIntPoint> ValidTiles;
	int32 MaxMovement;
	int32 MoveCost;
	TArray<FIntPoint> RedTiles;
	bool IsPlayerPathfinding;
		
	AGridManager* GridManager;
};

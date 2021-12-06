#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tile.generated.h"

class ATileActor;
class AProtoFECharacter;
class ATerrainModifier;

UENUM(BlueprintType)
enum class ETerrain : uint8
{
	Normal UMETA(DisplayName="Normal"),
	Foliage UMETA(DisplayName="Foliage"),
	Water UMETA(DisplayName="Water"),
	Mountains UMETA(DisplayName="Moutains"),
	Impossible UMETA(DisplayName="Impossible")
};

USTRUCT(BlueprintType)
struct FGridData
{
	GENERATED_USTRUCT_BODY()

	FGridData() {}
	FGridData(FIntPoint Coords)
	{
		Coordinates = Coords;
	}
	FGridData(FIntPoint Coords, int TileNumber) : FGridData(Coords)
	{
		this->TileNumber = TileNumber;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Coordinates = FIntPoint();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATileActor* TileActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATerrainModifier* TerrainMod = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AProtoFECharacter* OccupiedBy = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TileNumber = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETerrain Terrain = ETerrain::Normal;
	
};

UCLASS()
class PROTOFE_API UTile : public UObject
{
	GENERATED_BODY()

public:
	UTile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGridData Data = FGridData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pathfinding)
	int FinalCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pathfinding)
	int CostFromStart = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pathfinding)
	int EstimatedCostToTarget = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pathfinding)
	UTile* PreviousTile = nullptr;
	
};

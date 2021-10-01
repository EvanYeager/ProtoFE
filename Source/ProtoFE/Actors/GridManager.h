#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

class AProtoFECharacter;
class ATile;

UENUM(BlueprintType)
enum class ETerrain : uint8
{
	Normal UMETA(DisplayName="Normal"),
	Foliage UMETA(DisplayName="Foliage"),
	Water UMETA(DisplayName="Water"),
	Mountains UMETA(DisplayName="Moutains")	
};

USTRUCT(BlueprintType)
struct FGridData
{
	GENERATED_USTRUCT_BODY()

	FGridData() {}
	FGridData(int TileNumber)
	{
		this->TileNumber = TileNumber;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AProtoFECharacter* OccupiedBy = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TileNumber = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETerrain Terrain = ETerrain::Normal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pathfinding)
	int FinalCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pathfinding)
	int CostFromStart = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pathfinding)
	int EstimatedCostToTarget = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pathfinding)
	FIntPoint PreviousTileCoords = FIntPoint(0, 0);
	
};


UCLASS()
class PROTOFE_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	TMap<FIntPoint, FGridData> Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	int GridX = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	int GridY = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	bool ShowTileColorInEditor = true;
	

	UFUNCTION(BlueprintCallable)
	virtual void CreateGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TSubclassOf<ATile> TileClass;
	TArray<ATile*> TileActors;
	const int PlaneLength = 120;


};
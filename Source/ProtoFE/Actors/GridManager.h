#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

class AProtoFECharacter;
class ATileActor;
class ITerrainMod;
class UTile;


USTRUCT(BlueprintType)
struct FGridRow
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTile*> Tiles;
};


UCLASS()
class PROTOFE_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	TArray<FGridRow> Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	int GridX = 18;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	int GridY = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	bool ShowTileColorInEditor = true;
	
	const int PlaneLength = 120;

	UFUNCTION(BlueprintCallable)
	void CreateGrid();
	static UTile* GetTileWithLocation(UWorld* WorldContext, FVector Location);
	static UTile* GetTileWithCoords(FIntPoint Coords, TArray<FGridRow> Grid);
	static UTile* GetTileWithActor(AActor* Actor, AGridManager* GridManager);

	static TArray<FGridRow>* GetGrid(UWorld* WorldContext);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TSubclassOf<ATileActor> TileClass;



};
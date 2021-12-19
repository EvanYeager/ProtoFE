#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

class AProtoFECharacter;
class ATileActor;
class UTile;


USTRUCT(BlueprintType)
struct FGridColumn
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
	TArray<FGridColumn> Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	int GridX = 18;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	int GridY = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	bool ShowTileColorInEditor = true;

	const static int PlaneLength;

	UFUNCTION(BlueprintCallable)
	void CreateGrid();
	static UTile* GetTile(FIntPoint Coords, TArray<FGridColumn> Grid);
	static UTile* GetTile(FIntPoint Coords, UWorld* WorldContext);
	static UTile* GetTile(FVector Location, UWorld* WorldContext);
	UTile* GetTile(FVector Location);

	static AGridManager* GetGridManager(UWorld* WorldContext);
	static TArray<FGridColumn>* GetGrid(UWorld* WorldContext);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TSubclassOf<ATileActor> TileClass;



};
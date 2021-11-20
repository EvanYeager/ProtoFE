#include "TerrainModifier.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SnapToGrid.h"
#include "Kismet/GameplayStatics.h"
#include "Components/GridOccupyComponent.h"
#include "Actors/GridManager.h"


// Sets default values
ATerrainModifier::ATerrainModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	GridOccupyComponent = CreateDefaultSubobject<UGridOccupyComponent>(TEXT("Grid Occupy Component"));
	GridSnapComponent = CreateDefaultSubobject<USnapToGrid>(TEXT("Grid Snap Component"));

}

// Called when the game starts or when spawned
void ATerrainModifier::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATerrainModifier::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (!GridSnapComponent->SnapToGrid) return;

	if (bFinished)
	{
		GridSnapComponent->SnapToClosestTile();
	}
}

// Called every frame
void ATerrainModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATerrainModifier::OccupyTile(UTile* NewTile)
{
	TArray<FGridColumn>* Grid = AGridManager::GetGrid(GetWorld());

	if (GridOccupyComponent->OccupiedTile != nullptr) // if OccupiedTile is not the default
	{
		// delete from old tile
		GridOccupyComponent->OccupiedTile->Data.TerrainMod = nullptr;
		GridOccupyComponent->OccupiedTile->Data.Terrain = ETerrain::Normal;
	}
		
	// add on new tile
	NewTile->Data.TerrainMod = this;
	NewTile->Data.Terrain = Terrain;
	GridOccupyComponent->OccupiedTile = NewTile;
}
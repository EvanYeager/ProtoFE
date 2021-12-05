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
	PrimaryActorTick.bCanEverTick = false;

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

void ATerrainModifier::Destroyed()
{
	Super::Destroyed();

	DeleteFromCurrentTile();
}

void ATerrainModifier::DeleteFromCurrentTile()
{
	if (GridOccupyComponent->OccupiedTile)
	{
		GridOccupyComponent->OccupiedTile->Data.TerrainMod = nullptr;
		GridOccupyComponent->OccupiedTile->Data.Terrain = ETerrain::Normal;
		GridOccupyComponent->OccupiedTile = nullptr;
	}
}

void ATerrainModifier::OccupyNewTile(UTile* NewTile)
{
	NewTile->Data.TerrainMod = this;
	NewTile->Data.Terrain = Terrain;
	GridOccupyComponent->OccupiedTile = NewTile;
}


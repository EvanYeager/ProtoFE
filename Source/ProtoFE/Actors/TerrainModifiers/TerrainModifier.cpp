#include "TerrainModifier.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SnapToGrid.h"
#include "Kismet/GameplayStatics.h"
#include "Components/GridOccupyComponent.h"
#include "Actors/GridManager.h"
#include "Actors/TileActor.h"


// Sets default values
ATerrainModifier::ATerrainModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComponent);

	// Collision
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Mesh);
	Collision->InitBoxExtent(FVector(40)); // 40 is the default size of the collision for terrain modifiers that take up one grid tile
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);

	// GridOccupyComponent = CreateDefaultSubobject<UGridOccupyComponent>(TEXT("Grid Occupy Component"));
	GridSnapComponent = CreateDefaultSubobject<USnapToGrid>(TEXT("Grid Snap Component"));

}

// Called when the game starts or when spawned
void ATerrainModifier::BeginPlay()
{
	Super::BeginPlay();
	
	// occupy all tiles under the collision; ideally this would be done after moving the actor but it didn't work so this is a workaround
	DeleteFromCurrentTile();
	TArray<AActor*> Result;
	GetOverlappingActors(Result, ATileActor::StaticClass());
	for (AActor* Actor : Result)
	{
		if (ATileActor* Tile = Cast<ATileActor>(Actor))
		{
			OccupyNewTile(AGridManager::GetTile(Tile->GetActorLocation(), GetWorld()));
		}
	}
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
	for (UTile* Tile : OccupiedTiles)
	{
		Tile->Data.TerrainMod = nullptr;
		Tile->Data.Terrain = ETerrain::Normal;
	}
	OccupiedTiles.Empty();
}

void ATerrainModifier::OccupyNewTile(UTile* NewTile)
{
	NewTile->Data.TerrainMod = this;
	NewTile->Data.Terrain = Terrain;
	OccupiedTiles.AddUnique(NewTile);
}


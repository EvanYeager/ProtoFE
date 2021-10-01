#include "GridManager.h"
#include "Engine/World.h"
#include "UObject\ConstructorHelpers.h"
#include "Actors/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AActor> TileMesh(TEXT("/Game/TopDownCPP/Blueprints/Actors/TileBP"));
	if (TileMesh.Succeeded())
		TileClass = TileMesh.Class;
}


void AGridManager::OnConstruction(const FTransform& Transform) 
{
	for (ATile* Tile : TileActors)
	{
		Tile->Plane->SetVisibility(ShowTileColorInEditor);
	}
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::CreateGrid() 
{
	Grid.Empty();
	TileActors.Empty();
	// create grid data
	TMap<FIntPoint, FVector> Locations;
	int count = 0;
	for (int y = 1; y <= GridY; y++)
	{
		for (int x = 1; x <= GridX; x++)
		{
			Grid.Add(FIntPoint(x, y), FGridData(++count));
			FVector StartingLoc = GetActorLocation();
			Locations.Add(FIntPoint(x, y), FVector(StartingLoc.X + ((x - 1) * PlaneLength), StartingLoc.Y - ((y - 1) * PlaneLength), StartingLoc.Z));
		}
	}

	// remove existing tiles
	TArray<AActor*> Tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), Tiles);
	for (AActor* Tile : Tiles)
	{
		Tile->Destroy();
	}

	// spawn tiles
	if (TileClass)
	{
		count = 0;
		for (auto& Element : Locations)
		{
			FActorSpawnParameters SpawnParams = FActorSpawnParameters();
			// SpawnParams.Name = FName("Tile%i", ++count);
			TileActors.Add(GetWorld()->SpawnActor<ATile>(TileClass, Element.Value, FRotator(0, 0, 0), SpawnParams));
		}
	}

	// set color & whatnot
	for (ATile* Tile : TileActors)
	{
		Tile->Plane->SetVisibility(ShowTileColorInEditor);
	}
}

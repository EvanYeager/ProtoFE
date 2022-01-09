#include "GridManager.h"
#include "Engine/World.h"
#include "UObject\ConstructorHelpers.h"
#include "Actors/TileActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Tile.h"


#include "CoreMisc.h"


const int AGridManager::PlaneLength = 120;

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AActor> TileMesh(TEXT("/Game/TopDownCPP/Blueprints/Actors/TileActorBP"));
	if (TileMesh.Succeeded())
		TileClass = TileMesh.Class;
}


void AGridManager::OnConstruction(const FTransform& Transform) 
{
	// set visibility in editor
	for (FGridColumn Col : Grid)
	{
		for (UTile* Tile : Col.Tiles)
		{
			if (Tile->Data.TileActor)
				Tile->Data.TileActor->HighlightPlane->SetVisibility(ShowTileColorInEditor);
		}
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
	Grid.Empty(GridX);
	// holds the locations where the tile actors should be spawned
	TMap<UTile*, FVector> Locations;
	int count = 0;

	// create columns
	for (int col = 0; col < GridX; col++)
	{
		Grid.Add(FGridColumn());
	}
	// create tile data
	for (int y = 0; y < GridY; y++)
	{
		for (int x = 0; x < GridX; x++)
		{
			Grid[x].Tiles.Add(NewObject<UTile>(this));
			Grid[x].Tiles[y]->Data = FGridData(FIntPoint(x, y), ++count);
			FVector StartingLoc = GetActorLocation();
			Locations.Add(Grid[x].Tiles[y], FVector(StartingLoc.X + (x * PlaneLength), StartingLoc.Y - (y * PlaneLength), StartingLoc.Z));
		}
	}

	// remove existing tiles
	TArray<AActor*> Tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileActor::StaticClass(), Tiles);
	for (AActor* Tile : Tiles)
	{
		Tile->Destroy();
	}

	// spawn tiles
	if (TileClass)
	{
		count = 0;
		for (auto Element : Locations)
		{
			FActorSpawnParameters SpawnParams = FActorSpawnParameters();
			SpawnParams.Name = FName("Tile", count++);
			Element.Key->Data.TileActor = GetWorld()->SpawnActor<ATileActor>(TileClass, Element.Value, FRotator(0, 0, 0), SpawnParams);
		}
	
		// set color & whatnot
		for (FGridColumn Col : Grid)
		{
			for (UTile* Tile : Col.Tiles)
				Tile->Data.TileActor->HighlightPlane->SetVisibility(ShowTileColorInEditor);
		}
	}
}

UTile* AGridManager::GetTile(FIntPoint Coords, TArray<FGridColumn> Grid)
{
	if (Coords.X < 0 || Coords.Y < 0 || Grid.Num() <= Coords.X || Grid[0].Tiles.Num() <= Coords.Y || !Grid[Coords.X].Tiles[Coords.Y]) return nullptr;

	return Grid[Coords.X].Tiles[Coords.Y];
}

UTile* AGridManager::GetTile(FIntPoint Coords, UWorld* WorldContext)
{
	TArray<FGridColumn> Grid = *AGridManager::GetGrid(WorldContext);
	return AGridManager::GetTile(Coords, Grid);
}

UTile* AGridManager::GetTile(FVector Location, UWorld* WorldContext)
{
	AGridManager* GridManager = AGridManager::GetGridManager(WorldContext);
	return GridManager->GetTile(Location);
}

UTile* AGridManager::GetTile(FVector Location)
{
	FVector2D DistanceFromRoot = FVector2D( // distance from given location to grid manager
		GetActorLocation().X - Location.X,
		GetActorLocation().Y - Location.Y
	);
	FIntPoint TilesAway = FIntPoint( // tiles away from the root, aka grid manager
		FMath::RoundToInt(FMath::Abs(DistanceFromRoot.X / PlaneLength)),
		FMath::RoundToInt(FMath::Abs(DistanceFromRoot.Y / PlaneLength))
	);
	
	return AGridManager::GetTile(TilesAway, Grid);
}


AGridManager* AGridManager::GetGridManager(UWorld* WorldContext)
{
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, AGridManager::StaticClass(), Temp);
	return Cast<AGridManager>(Temp[0]);
}

TArray<FGridColumn>* AGridManager::GetGrid(UWorld* WorldContext)
{
	return &GetGridManager(WorldContext)->Grid;
}
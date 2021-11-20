#include "GridManager.h"
#include "Engine/World.h"
#include "UObject\ConstructorHelpers.h"
#include "Actors/TileActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Tile.h"


#include "CoreMisc.h"


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
	for (FGridRow Row : Grid)
	{
		for (UTile* Tile : Row.Tiles)
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
	Grid.Empty(GridY);
	TMap<UTile*, FVector> Locations;
	int count = 0;
	// create vars
	for (int y = 0; y < GridY; y++)
	{
		Grid.Add(FGridRow());
		for (int x = 0; x < GridX; x++)
		{
			Grid[y].Tiles.Add(NewObject<UTile>(this));
			Grid[y].Tiles[x]->Data = FGridData(FIntPoint(x, y), ++count);
			FVector StartingLoc = GetActorLocation();
			Locations.Add(Grid[y].Tiles[x], FVector(StartingLoc.X + (x * PlaneLength), StartingLoc.Y - (y * PlaneLength), StartingLoc.Z));
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
			Element.Key->Data.TileActor = GetWorld()->SpawnActor<ATileActor>(TileClass, Element.Value, FRotator(0, 0, 0), SpawnParams);
		}
	
		// set color & whatnot
		for (FGridRow Row : Grid)
		{
			for (UTile* Tile : Row.Tiles)
			Tile->Data.TileActor->HighlightPlane->SetVisibility(ShowTileColorInEditor);
		}
	}
}

UTile* AGridManager::GetTileWithLocation(UWorld* WorldContext, FVector Location)
{
	for (FGridRow Row : *AGridManager::GetGrid(WorldContext))
	{
		for (UTile* Tile : Row.Tiles)
		{
			if (Location.Equals(Tile->Data.TileActor->GetActorLocation(), 40.0f))
				return Tile;
		}
	}
	return nullptr;
}

UTile* AGridManager::GetTileWithCoords(FIntPoint Coords, TArray<FGridRow> Grid)
{
	if (Coords.X < 0 || Coords.Y < 0 || Grid.Num() < Coords.Y || !Grid[Coords.Y].Tiles[Coords.X]) return nullptr;
	return Grid[Coords.Y].Tiles[Coords.X];
}

UTile* AGridManager::GetTileWithActor(AActor* Actor, AGridManager* GridManager)
{
	FVector TileLocation = Actor->GetActorLocation();
	FVector2D DistanceFromRoot = FVector2D( // distance from actor to grid manager
		GridManager->GetActorLocation().X - TileLocation.X,
		GridManager->GetActorLocation().Y - TileLocation.Y
	);
	FIntPoint TilesAway = FIntPoint( // tiles away from the root, aka grid manager
		FMath::RoundToInt(FMath::Abs(DistanceFromRoot.X / GridManager->PlaneLength)),
		FMath::RoundToInt(FMath::Abs(DistanceFromRoot.Y / GridManager->PlaneLength))
	);
	
	return AGridManager::GetTileWithCoords(TilesAway, GridManager->Grid);
}

TArray<FGridRow>* AGridManager::GetGrid(UWorld* WorldContext)
{
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, AGridManager::StaticClass(), Temp);
	return &Cast<AGridManager>(Temp[0])->Grid;
}
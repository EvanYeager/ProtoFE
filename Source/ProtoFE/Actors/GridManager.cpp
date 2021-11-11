#include "GridManager.h"
#include "Engine/World.h"
#include "UObject\ConstructorHelpers.h"
#include "Actors/TileActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"


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
	for (auto& Element : Grid)
	{
		if (Element.Value.Tile)
			Element.Value.Tile->Plane->SetVisibility(ShowTileColorInEditor);
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
	TMap<FIntPoint, FVector> Locations;
	int count = 0;
	// create vars
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileActor::StaticClass(), Tiles);
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
			Grid.Find(Element.Key)->Tile = GetWorld()->SpawnActor<ATileActor>(TileClass, Element.Value, FRotator(0, 0, 0), SpawnParams);
		}
	
		// set color & whatnot
		for (auto& Element : Grid)
		{
			Element.Value.Tile->Plane->SetVisibility(ShowTileColorInEditor);
		}
	}
}

FIntPoint AGridManager::GetTileWithLocation(UWorld* WorldContext, FVector Location)
{
	for (auto& Tile : *AGridManager::GetGrid(WorldContext))
	{
		if (Location.Equals(Tile.Value.Tile->GetActorLocation(), 40.0f))
			return Tile.Key;
	}
	return FIntPoint(0, 0);
}

TMap<FIntPoint, FGridData>* AGridManager::GetGrid(UWorld* WorldContext)
{
	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, AGridManager::StaticClass(), Temp);
	if (Temp.Num() == 0) return new TMap<FIntPoint, FGridData>();
	return &Cast<AGridManager>(Temp[0])->Grid;
}
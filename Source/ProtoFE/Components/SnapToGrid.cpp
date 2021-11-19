#include "SnapToGrid.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/GridManager.h"
#include "Actors/TileActor.h"
#include "Actors/TerrainModifiers/TerrainMod.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tile.h"

// Sets default values for this component's properties
USnapToGrid::USnapToGrid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USnapToGrid::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USnapToGrid::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USnapToGrid::SnapToClosestTile()
{
	if (!SnapToGrid) return;

	TArray<AActor*> Temp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGridManager::StaticClass(), Temp);
	if (Temp.Num() == 0) return;
	GridManager = Cast<AGridManager>(Temp[0]);

	FVector TrueLocation = GetOwner()->GetActorLocation();
	FVector2D DistanceFromRoot = FVector2D( // distance from actor to grid manager
		GridManager->GetActorLocation().X - TrueLocation.X,
		GridManager->GetActorLocation().Y - TrueLocation.Y
	);
	FIntPoint TilesAway = FIntPoint( // tiles away from the root, aka grid manager
		FMath::RoundToInt(FMath::Abs(DistanceFromRoot.X / GridManager->PlaneLength)),
		FMath::RoundToInt(FMath::Abs(DistanceFromRoot.Y / GridManager->PlaneLength))
	);
	FIntPoint TileCoords = TilesAway;
	
	UTile* NewTile = AGridManager::GetTileWithCoords(TileCoords, GridManager->Grid);
	if (!NewTile) return;

	// snap actor to closest tile
	GetOwner()->SetActorLocation(NewTile->Data.TileActor->GetActorLocation()); 

	if (IGridOccupy* OwnerAsGridOccupy = Cast<IGridOccupy>(GetOwner()))
		OwnerAsGridOccupy->OccupyTile(NewTile);
}


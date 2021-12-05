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

	// find closest tile
	UTile* NewTile = AGridManager::GetTile(GetOwner()->GetActorLocation(), GetWorld());
	if (!NewTile) return;

	// snap actor to closest tile
	GetOwner()->SetActorLocation(NewTile->Data.TileActor->GetActorLocation()); 

	if (IGridOccupy* OwnerAsGridOccupy = Cast<IGridOccupy>(GetOwner()))
		OwnerAsGridOccupy->MoveTiles(NewTile);
}


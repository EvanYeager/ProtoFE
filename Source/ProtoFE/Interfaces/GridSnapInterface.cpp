#include "GridSnapInterface.h"
#include "Actors/TerrainModifiers/TerrainMod.h"
#include "Actors/GridManager.h"
#include "Kismet/GameplayStatics.h"

void IGridSnapInterface::AddTerrainModToTile(ITerrainMod* TerrainMod, FIntPoint Tile)
{
   if (!GridManager)
   {
      GridManager = FindGridManager();
      if (!GridManager) return;
   }

   FGridData* TileData = GridManager->Grid.Find(Tile);
   if (!TileData) return;

   AActor* ThisTerrainAsActor = Cast<AActor>(TerrainMod);
   TileData->TerrainMod = ThisTerrainAsActor;
   TileData->Terrain = TerrainMod->Terrain;
}

void IGridSnapInterface::DeleteTerrainModFromGrid(ITerrainMod* TerrainMod)
{
   if (!GridManager)
   {
      GridManager = FindGridManager();
      if (!GridManager) return;
   }

	for (auto& Tile : GridManager->Grid)
	{
		if (Tile.Value.TerrainMod == TerrainMod) 
		{
			Tile.Value.TerrainMod = nullptr;
			Tile.Value.Terrain = ETerrain::Normal;
			return;
		}
	}
}

AGridManager* IGridSnapInterface::FindGridManager()
{
   if (UActorComponent* ThisAsActorComp = Cast<UActorComponent>(this))
   {
      TArray<AActor*> Temp;
      UGameplayStatics::GetAllActorsOfClass(ThisAsActorComp->GetOwner()->GetWorld(), AGridManager::StaticClass(), Temp);
      if (Temp.Num() > 0) return Cast<AGridManager>(Temp[0]);
      else return nullptr;
   }
   return nullptr;
}
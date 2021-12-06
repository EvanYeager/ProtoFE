#include "GridSnapInterface.h"
#include "Actors/TerrainModifiers/TerrainModifier.h"
#include "Actors/GridManager.h"
#include "Kismet/GameplayStatics.h"
#include "Tile.h"

void IGridSnapInterface::AddTerrainModToTile(ATerrainModifier* TerrainMod, UTile* Tile)
{
   if (!Tile) return;

   Tile->Data.TerrainMod = TerrainMod;
   Tile->Data.Terrain = TerrainMod->Terrain;
}

void IGridSnapInterface::DeleteTerrainModFromGrid(ATerrainModifier* TerrainMod)
{
   if (!GridManager)
   {
      GridManager = FindGridManager();
      if (!GridManager) return;
   }

	for (FGridColumn Col : GridManager->Grid)
	{
      for (UTile* Tile : Col.Tiles)
      {
         if (Tile->Data.TerrainMod == TerrainMod) 
         {
            Tile->Data.TerrainMod = nullptr;
            Tile->Data.Terrain = ETerrain::Normal;
            return;
         }
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
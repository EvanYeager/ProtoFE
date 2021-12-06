

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridSnapInterface.generated.h"

class ATerrainModifier;
class AGridManager;
class UTile;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGridSnapInterface : public UInterface
{
	GENERATED_BODY()
};

class PROTOFE_API IGridSnapInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	AGridManager* GridManager;

	void AddTerrainModToTile(ATerrainModifier* TerrainMod, UTile* Tile);
	void DeleteTerrainModFromGrid(ATerrainModifier* TerrainMod);
	AGridManager* FindGridManager();

protected:

};

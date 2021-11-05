

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Actors/GridManager.h"
#include "Interfaces/GridOccupy.h"
#include "TerrainMod.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTerrainMod : public UInterface
{
	GENERATED_BODY()
};

class PROTOFE_API ITerrainMod
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	ETerrain Terrain = ETerrain::Normal;
	
protected:

private:
	
};

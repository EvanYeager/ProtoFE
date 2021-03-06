#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridOccupy.generated.h"

class UTile;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGridOccupy : public UInterface
{
	GENERATED_BODY()
};

class PROTOFE_API IGridOccupy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Deletes the previously occupied tile, and sets on the new one */
	virtual void MoveTiles(UTile* NewTile);

	virtual void DeleteFromCurrentTile() = 0;
	virtual void OccupyNewTile(UTile* NewTile) = 0;

};

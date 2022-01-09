

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROTOFE_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** give item by pointer. Uses an existing item */
	virtual void GiveItem(UItem* Item) = 0;
	/** give item by class. Constructs a new item */
	virtual void GiveItem(TSubclassOf<UItem> ItemClass) = 0;
	/** destroy or take item out of inventory */
	virtual void RemoveItem(UItem* Item) = 0;
	virtual void SetAsFirst(UItem* Item) = 0;
};

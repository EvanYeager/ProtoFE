

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Consumable.generated.h"

/**
 * 
 */
UCLASS()
class PROTOFE_API UConsumable : public UItem
{
	GENERATED_BODY()

public:
	UConsumable();

	virtual void Use() {}

protected:

private:
	
};

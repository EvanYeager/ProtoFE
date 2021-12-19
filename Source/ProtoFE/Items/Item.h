

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROTOFE_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UItem();

	int Durability;
	FString Name = "Item";


	// TODO item portrait

	

protected:

private:

	
};

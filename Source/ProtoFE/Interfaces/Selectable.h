

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Selectable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USelectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROTOFE_API ISelectable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// defines what to do when actor is selected
	virtual void Select() = 0;
};

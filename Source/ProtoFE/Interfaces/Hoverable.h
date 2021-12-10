

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hoverable.generated.h"

class UPrimitiveComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHoverable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROTOFE_API IHoverable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnCursorOver(UPrimitiveComponent* comp) = 0;
	virtual void EndCursorOver(UPrimitiveComponent* comp) = 0;
	virtual void CreateToolTipWindow();
};

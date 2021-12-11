

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarParent.generated.h"

class AProtoFECharacter;

/**
 * 
 */
UCLASS()
class PROTOFE_API UHealthBarParent : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	AProtoFECharacter* Char;

protected:
	UPROPERTY(BlueprintReadWrite)
	float Percent;

	
};

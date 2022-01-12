

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolTipParent.generated.h"

class AProtoFECharacter;

/**
 * 
 */
UCLASS()
class PROTOFE_API UToolTipParent : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FLinearColor BackgroundColor = FLinearColor(.01, .01, .01, .9);

	UPROPERTY(BlueprintReadWrite)
	AProtoFECharacter* FocusedChar;

	UFUNCTION(BlueprintCallable)
	void Test();
	
};

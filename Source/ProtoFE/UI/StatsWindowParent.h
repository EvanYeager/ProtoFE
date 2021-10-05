#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProtoFECharacter.h"
#include "StatsWindowParent.generated.h"


UCLASS()
class PROTOFE_API UStatsWindowParent : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterInfo Info = FCharacterInfo();
	
};



#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProtoFEData.h"
#include "MyGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROTOFE_API UMyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMyGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EProtoFEAbilityInputID AbilityInputID = EProtoFEAbilityInputID::None;  	
	
};

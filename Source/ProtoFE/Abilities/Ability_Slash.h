

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "Ability_Slash.generated.h"

/**
 * 
 */
UCLASS()
class PROTOFE_API UAbility_Slash : public UAbility
{
	GENERATED_BODY()

public:
	UAbility_Slash();
	static FString Name;

	virtual bool TryActivateAbility() override;
	virtual void Activate() override;
};

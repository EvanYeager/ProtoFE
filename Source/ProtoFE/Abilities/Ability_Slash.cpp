


#include "Ability_Slash.h"

FString UAbility_Slash::Name = "Slash";

UAbility_Slash::UAbility_Slash()
{
	Cost = 2;
}

bool UAbility_Slash::TryActivateAbility()
{
	return Super::TryActivateAbility();
}

void UAbility_Slash::Activate()
{
		
}




#include "AbilityComponent.h"

#include "ProtoFECharacter.h"
#include "Abilities/Ability.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAbilityComponent::GiveAbility(TSubclassOf<UAbility> AbilityClass)
{
	if (HasLearnedAbility(AbilityClass)) return;
	
	UAbility* NewAbility = NewObject<UAbility>(this, AbilityClass);
	NewAbility->Owner = Cast<AProtoFECharacter>(GetOwner());
	LearnedAbilities.Add(NewAbility);
	if (AbilityHotbar.Num() < HOTBAR_SIZE)
		EquipAbility(NewAbility);
}

void UAbilityComponent::EquipAbility(UAbility* Ability)
{
	if (!Ability || AbilityHotbar.Num() >= HOTBAR_SIZE) return;
	
	AddAbilityToFirstSlot(Ability);
}

void UAbilityComponent::UnequipAbility(UAbility* Ability)
{
	// O(n)... oh well
	for (int i = 1; i <= HOTBAR_SIZE; i++)
	{
		if (AbilityHotbar.Find(i) && *AbilityHotbar.Find(i) == Ability)
		{
			AbilityHotbar.Remove(i);
			return;
		}
	}
}

bool UAbilityComponent::HasLearnedAbility(TSubclassOf<UAbility> AbilityClass)
{
	for (UAbility* Ability : LearnedAbilities)
	{
		if (Ability->StaticClass() == AbilityClass) return true;
	}
	return false;
}

bool UAbilityComponent::AddAbilityToFirstSlot(UAbility* Ability)
{
	for (int i = 1; i <= HOTBAR_SIZE; i++)
	{
		if (!AbilityHotbar.Find(i))
		{
			AbilityHotbar.Add(i, Ability);
			return true;
		}
	}
	return false;
}
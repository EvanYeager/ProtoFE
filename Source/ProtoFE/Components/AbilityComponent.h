

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UAbility;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOFE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

	/** Equivalent to the character "learning" an ability */
	void GiveAbility(TSubclassOf<UAbility> AbilityClass);
	/** Puts the ability in first empty hotbar slot, if available */
	void EquipAbility(UAbility* Ability);
	/** Removes the ability from hotbar */
	void UnequipAbility(UAbility* Ability);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<UAbility*> GetLearnedAbilities() const { return LearnedAbilities; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TMap<int, UAbility*> GetAbilityHotbar() const { return AbilityHotbar; }
	
protected:

private:
	UPROPERTY()
	TArray<UAbility*> LearnedAbilities;

	UPROPERTY(VisibleAnywhere)
	TMap<int, UAbility*> AbilityHotbar;
	
	bool HasLearnedAbility(TSubclassOf<UAbility> AbilityClass);
	bool AddAbilityToFirstSlot(UAbility* Ability);

	int HOTBAR_SIZE = 10;
		
};

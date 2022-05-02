


#include "MyGameplayEffect.h"

#include "MyAttributeSet.h"

UMyGameplayEffect::UMyGameplayEffect()
{
	FGameplayModifierInfo DamageEffect;
	DamageEffect.Attribute = UMyAttributeSet::GetHealthAttribute();
	DamageEffect.ModifierOp = EGameplayModOp::Additive;
	DamageEffect.ModifierMagnitude = FGameplayEffectModifierMagnitude(10.0f);
	Modifiers.Add(DamageEffect);
}

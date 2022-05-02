

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "MyAttributeSet.generated.h"


// Uses macros from Attributes.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROTOFE_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMyAttributeSet();
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Magic;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Magic);

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Skill;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Skill);

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Speed);

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Luck);

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Defense);

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Resistance;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Resistance);
	
};

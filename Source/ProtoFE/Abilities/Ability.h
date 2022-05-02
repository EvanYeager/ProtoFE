

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"

class AProtoFECharacter;

UENUM(BlueprintType)
enum class ETarget : uint8
{
	Self UMETA(DisplayName="Self"),
	Enemy UMETA(DisplayName="Enemy"),
	Ally UMETA(DisplayName="Ally")
};

UCLASS()
class PROTOFE_API UAbility : public UObject
{
	GENERATED_BODY()
	
public:
	UAbility();
	
	/** Invokes ability if it can. Override this for custom checks.
	 *@return if the ability was activated */
	virtual bool TryActivateAbility();
	/** Activates ability. Should be overriden in every ability class. */
	virtual void Activate();

	static FString Name;

	UPROPERTY()
	AProtoFECharacter* Owner;
	int Cost = 1;

	UFUNCTION(BlueprintCallable, Category="Ability")
	static FString GetName();

protected:
	/** determines the target of the ability */
	ETarget Target = ETarget::Enemy;
};

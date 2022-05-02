

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProtoFEData.generated.h"

UENUM(BlueprintType)
enum class EProtoFEAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Basic
};

UCLASS()
class PROTOFE_API UProtoFEData : public UObject
{
	GENERATED_BODY()
	
};

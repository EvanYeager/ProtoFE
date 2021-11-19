#pragma once

#include "CoreMinimal.h"
#include "ProtoFECharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS(Blueprintable)
class PROTOFE_API APlayerCharacter : public AProtoFECharacter
{
	GENERATED_BODY()


public:
	APlayerCharacter();
	
	virtual void Select() override;
	
};

#pragma once

#include "CoreMinimal.h"
#include "ProtoFECharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PROTOFE_API AEnemyCharacter : public AProtoFECharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	virtual void SelectCharacter() override;
};

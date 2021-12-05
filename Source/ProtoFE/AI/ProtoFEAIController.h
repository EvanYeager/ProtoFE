

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ProtoFEAIController.generated.h"

class UTile;

UCLASS()
class PROTOFE_API AProtoFEAIController : public AAIController
{
	GENERATED_BODY()

public:
	AProtoFEAIController();
	void MoveCharacter(TArray<UTile*> Path);

	TArray<UTile*> Path;

protected:
	void BeginPlay() override;

private:
	class UBehaviorTree* AIBehavior;
	
};

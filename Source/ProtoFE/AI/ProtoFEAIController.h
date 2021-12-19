

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
	void MoveCharacter(TArray<UTile*> path);
	void OnMoveFinished();

	TArray<UTile*> Path;

protected:
	void BeginPlay() override;

private:
	class UBehaviorTree* AIBehavior;
	
};

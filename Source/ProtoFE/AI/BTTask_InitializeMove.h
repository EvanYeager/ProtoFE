

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InitializeMove.generated.h"

UCLASS()
class PROTOFE_API UBTTask_InitializeMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_InitializeMove();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

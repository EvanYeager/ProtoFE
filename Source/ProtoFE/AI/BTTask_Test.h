

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveDirectlyToward.h"
#include "BTTask_Test.generated.h"

/**
 * 
 */
UCLASS()
class PROTOFE_API UBTTask_Test : public UBTTask_MoveDirectlyToward
{
	GENERATED_BODY()

public:
	UBTTask_Test();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};



#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetNewLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROTOFE_API UBTTask_SetNewLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetNewLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void IncrementMoveCount(UBehaviorTreeComponent& OwnerComp);

};

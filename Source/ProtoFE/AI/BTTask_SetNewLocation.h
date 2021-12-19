

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetNewLocation.generated.h"

class UTile;
class AProtoFEAIController;

UENUM(BlueprintType)
enum class EPathingDirection : uint8
{
	Reset UMETA(DisplayName="Reset"),
	Horizontal UMETA(DisplayName="Horizontal"),
	Vertical UMETA(DisplayName="Vertical"),
};

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

	/** This allows me to, if the path has more than one tile in a row going in the same direction, choose the last tile before the path turns the other way (or ends) */
	UTile* FindTargetTile(AProtoFEAIController* AIController, int MoveCount);

	int TilesMovedInOneGo = 1;

};

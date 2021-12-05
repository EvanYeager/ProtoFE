#include "BTTask_SetNewLocation.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tile.h"
#include "ProtoFEPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/TileActor.h"
#include "ProtoFEAIController.h"

UBTTask_SetNewLocation::UBTTask_SetNewLocation() 
{
   NodeName = "Set New Location";
}

EBTNodeResult::Type UBTTask_SetNewLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
   Super::ExecuteTask(OwnerComp, NodeMemory);

   UE_LOG(LogTemp, Warning, TEXT("in update task"));

   IncrementMoveCount(OwnerComp);

   int MoveCount = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("TilesMovedTo"));
   int PathLength = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("NumOfTilesInPath"));
   if (MoveCount >= PathLength) // if character reached final tile
   {
      OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("MoveLocation"));
      OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TilesMovedTo"));
      OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("NumOfTilesInPath"));
      UE_LOG(LogTemp, Warning, TEXT("moving is complete"));
      OwnerComp.StopLogic("moving is complete");
      return EBTNodeResult::Failed;
   }
   else
   {
      UE_LOG(LogTemp, Warning, TEXT("Finding next location. Move count is: %i"), MoveCount);
      AProtoFEAIController* AIController = Cast<AProtoFEAIController>(OwnerComp.GetAIOwner());
      OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("MoveLocation"), AIController->Path[MoveCount]->Data.TileActor->GetActorLocation());
   }
   return EBTNodeResult::Succeeded;

}

void UBTTask_SetNewLocation::IncrementMoveCount(UBehaviorTreeComponent& OwnerComp) 
{
   int initial = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("TilesMovedTo"));
   OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("TilesMovedTo"), ++initial);
}

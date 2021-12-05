#include "BTTask_Test.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tile.h"
#include "ProtoFEPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/TileActor.h"
#include "ProtoFEAIController.h"


UBTTask_Test::UBTTask_Test() 
{
   NodeName = TEXT("Test");
}

EBTNodeResult::Type UBTTask_Test::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
   Super::ExecuteTask(OwnerComp, NodeMemory);

   UE_LOG(LogTemp, Warning, TEXT("in test task"));

   // increment move counter
   OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("TilesMovedTo"), OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("TilesMovedTo")) + 1);

   int MoveCount = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("TilesMovedTo"));
   int PathLength = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("NumOfTilesInPath"));
   if (MoveCount >= PathLength)
   {
      OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("MoveLocation"));
      OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TilesMovedTo"));
      OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("NumOfTilesInPath"));
   }
   else
   {
      UE_LOG(LogTemp, Warning, TEXT("Finding next location"));
      AProtoFEAIController* AIController = Cast<AProtoFEAIController>(OwnerComp.GetAIOwner());
      OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("MoveLocation"), AIController->Path[MoveCount]->Data.TileActor->GetActorLocation());
   }
   return EBTNodeResult::Succeeded;

}

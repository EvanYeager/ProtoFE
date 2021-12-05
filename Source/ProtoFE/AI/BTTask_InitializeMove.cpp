#include "BTTask_InitializeMove.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tile.h"
#include "ProtoFEPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/TileActor.h"
#include "ProtoFEAIController.h"

UBTTask_InitializeMove::UBTTask_InitializeMove() 
{
   NodeName = "Initialize Move";
}

EBTNodeResult::Type UBTTask_InitializeMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
   AProtoFEAIController* AIController = Cast<AProtoFEAIController>(OwnerComp.GetAIOwner());
   if (AIController->Path.Num() == 0) return EBTNodeResult::Succeeded;

   FVector MoveLoc = AIController->Path[0]->Data.TileActor->GetActorLocation();
   MoveLoc.Z = AIController->GetPawn()->GetActorLocation().Z;
   OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("MoveLocation"), MoveLoc);
   UE_LOG(LogTemp, Warning, TEXT("initializing move. Path num is: %i; move location is: %s"), AIController->Path.Num(), *MoveLoc.ToString());
   OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("TilesMovedTo"), 0);
   OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("NumOfTilesInPath"), AIController->Path.Num());
   return EBTNodeResult::Succeeded;
}


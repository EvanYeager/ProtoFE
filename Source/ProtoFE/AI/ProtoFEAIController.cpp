#include "ProtoFEAIController.h"
#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Tile.h"


#include "Actors/TileActor.h"

AProtoFEAIController::AProtoFEAIController() 
{
   ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeObj(TEXT("BehaviorTree'/Game/TopDownCPP/Blueprints/AI/BT_CharacterAI.BT_CharacterAI'"));
	if (BehaviorTreeObj.Succeeded())
		AIBehavior = BehaviorTreeObj.Object;
}

void AProtoFEAIController::BeginPlay() 
{
   Super::BeginPlay();

   
}

void AProtoFEAIController::MoveCharacter(TArray<UTile*> Path) 
{
   if (AIBehavior)
   {
      this->Path = Path;
      RunBehaviorTree(AIBehavior);
   }
}

// void AProtoFEPlayerController::SetNewMoveDestination(const FVector DestLocation)
// {
// 	APawn* const MyPawn = GetPawn();
// 	if (MyPawn)
// 	{
// 		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

// 		// We need to issue move command only if far enough in order for walk animation to play correctly
// 		if ((Distance > 120.0f))
// 		{
// 			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
// 		}
// 	}
// }

// void AProtoFEPlayerController::OnSetDestinationPressed()
// {
// 	// set flag to keep updating destination until released
// 	bMoveToMouseCursor = true;
// }

// void AProtoFEPlayerController::OnSetDestinationReleased()
// {
// 	// clear flag to indicate we should stop updating the destination
// 	bMoveToMouseCursor = false;
// }

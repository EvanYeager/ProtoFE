#include "ProtoFEAIController.h"
#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Tile.h"
#include "ProtoFECharacter.h"
#include "Components/GridOccupyComponent.h"



AProtoFEAIController::AProtoFEAIController() 
{
   ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeObj(TEXT("BehaviorTree'/Game/TopDownCPP/Blueprints/AI/BT_CharacterAI.BT_CharacterAI'"));
	if (BehaviorTreeObj.Succeeded())
		AIBehavior = BehaviorTreeObj.Object;
}

void AProtoFEAIController::BeginPlay() 
{
   Super::BeginPlay();

   Char = Cast<AProtoFECharacter>(GetCharacter());   
}

void AProtoFEAIController::MoveCharacter(TArray<UTile*> path) 
{
   if (AIBehavior)
   {
      this->Path = path;
      RunBehaviorTree(AIBehavior);
      // subtract remaining movement
      Cast<AProtoFECharacter>(GetPawn())->RemainingMovement -= path[path.Num() - 1]->FinalCost - 1;
   }
}

void AProtoFEAIController::OnMoveFinished() 
{
   if (Path.Num() > 0)
   {
      Char->GridOccupyComponent->OccupiedTile = Path[Path.Num() - 1];
   }
}
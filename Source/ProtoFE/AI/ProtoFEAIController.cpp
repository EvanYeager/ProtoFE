#include "ProtoFEAIController.h"
#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Tile.h"
#include "ProtoFECharacter.h"
#include "Components/GridOccupyComponent.h"


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

void AProtoFEAIController::MoveCharacter(TArray<UTile*> path) 
{
   if (AIBehavior)
   {
      this->Path = path;
      RunBehaviorTree(AIBehavior);
   }
}

void AProtoFEAIController::OnMoveFinished() 
{
   if (Path.Num() > 0)
   {
      AProtoFECharacter* Char = Cast<AProtoFECharacter>(GetCharacter());
      Char->GridOccupyComponent->OccupiedTile = Path[Path.Num() - 1];
   }
}
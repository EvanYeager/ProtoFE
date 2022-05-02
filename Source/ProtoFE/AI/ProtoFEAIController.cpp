// ReSharper disable All
#include "ProtoFEAIController.h"
#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Tile.h"
#include "ProtoFECharacter.h"
#include "Components/GridOccupyComponent.h"
#include "Abilities/Ability.h"



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

// void AProtoFEAIController::AddAbility(UAbility* Ability) 
// {
//    Char->LearnedAbilities.AddUnique(Ability);
// }

// bool AProtoFEAIController::EquipAbility(UAbility* NewAbility) 
// {
//    if (!Char->LearnedAbilities.Contains(NewAbility))
//       return false;

//    for (int i = 1; i <= Char->NumOfSlots; i++)
//    {
//       if (Char->EquippedAbilities.Find(i)) continue; // if slot is full
//       Char->EquippedAbilities.Add(i, NewAbility);
//       return true;
//    }
//    return false;
// }

// bool AProtoFEAIController::EquipAbility(UAbility* NewAbility, int Slot) 
// {
//    if (!Char->LearnedAbilities.Contains(NewAbility) || Slot < 1 || Slot > 9)
//       return false;

//    // if there is a skill in that slot, remove it
//    if (Char->EquippedAbilities.Find(Slot))
//       Char->EquippedAbilities.Remove(Slot);

//    Char->EquippedAbilities.Add(Slot, NewAbility);
//    return true;
// }

// bool AProtoFEAIController::UnequipAbility(UAbility* Ability) 
// {
//    for (int i = 1; i <= Char->NumOfSlots; i++)
//    {
//       if (*Char->EquippedAbilities.Find(i) == Ability)
//       { 
//          Char->EquippedAbilities.Remove(i);
//          return true;
//       }
//    }
//    return false;
// }
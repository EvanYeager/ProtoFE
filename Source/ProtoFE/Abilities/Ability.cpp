#include "Ability.h"

#include "ProtoFECharacter.h"


FString UAbility::Name = "Base Ability";


UAbility::UAbility()
{
   // this->Owner = Owner;
}

// I think I need to change it to be an ability is instantiated when giving it to a character.
// So no static variables, and in the constructor every ability needs to know the owner.

bool UAbility::TryActivateAbility()
{
   if (true) // TODO if owner AP >= Cost
   {
      Activate();
      return true;
   }
   else
      return false;
}

void UAbility::Activate()
{
   UE_LOG(LogTemp, Warning, TEXT("owner is %s"), Owner? *Owner->GetName() : TEXT("not found"));
}

FString UAbility::GetName()
{
   return Name;
}

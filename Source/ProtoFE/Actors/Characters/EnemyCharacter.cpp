#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"


AEnemyCharacter::AEnemyCharacter() 
{
   Information.Team = ETeam::Enemy;   
   Information.Name = TEXT("Enemy");
}

void AEnemyCharacter::SelectCharacter() 
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      
   }
}

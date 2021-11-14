#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/GridManager.h"
#include "Actors/TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Tile.h"


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

void AEnemyCharacter::OnCursorOver(UPrimitiveComponent* comp)
{
   Super::OnCursorOver(comp);

   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      TArray<UTile*> RedTiles;
      TArray<AProtoFECharacter*> Chars;
      TArray<UTile*> MovementTiles = PlayerController->Pathfinder->BreadthSearch(this, RedTiles, Chars);
      MovementTiles.Append(RedTiles);
      for (UTile* Tile : MovementTiles)
      {
         Tile->Data.TileActor->Plane->SetVisibility(true);
         Tile->Data.TileActor->SetColor(EHighlightColor::EnemyRange);
         Tile->Data.TileActor->SetStrength(EHighlightStrength::Weak);
      }
   }
}

void AEnemyCharacter::EndCursorOver(UPrimitiveComponent* comp)
{
   Super::EndCursorOver(comp);
}

#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/GridManager.h"
#include "Actors/TileActor.h"
#include "Components/StaticMeshComponent.h"


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
      TArray<FIntPoint> RedTiles;
      TArray<AProtoFECharacter*> Chars;
      TArray<FIntPoint> MovementTiles = PlayerController->Pathfinder->BreadthSearch(this, RedTiles, Chars);
      MovementTiles.Append(RedTiles);
      TMap<FIntPoint, FGridData>* Grid = AGridManager::GetGrid(GetWorld());
      for (FIntPoint Tile : MovementTiles)
      {
         FGridData* TileInfo = Grid->Find(Tile);
         TileInfo->TileActor->Plane->SetVisibility(true);
         TileInfo->TileActor->SetColor(EHighlightColor::EnemyRange);
         TileInfo->TileActor->SetStrength(EHighlightStrength::Weak);
      }
   }
}

void AEnemyCharacter::EndCursorOver(UPrimitiveComponent* comp)
{
   Super::EndCursorOver(comp);
}

#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/GridManager.h"
#include "Actors/Tile.h"
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
      UE_LOG(LogTemp, Warning, TEXT("%i"), MovementTiles.Num());
      TMap<FIntPoint, FGridData>* Grid = AGridManager::GetGrid(GetWorld());
      for (FIntPoint Tile : MovementTiles)
      {
         FGridData* TileInfo = Grid->Find(Tile);
         TileInfo->Tile->Plane->SetVisibility(true);
         TileInfo->Tile->SetColor(EHighlightColor::EnemyRange);
         TileInfo->Tile->SetStrength(EHighlightStrength::Weak);
      }
   }
}

void AEnemyCharacter::EndCursorOver(UPrimitiveComponent* comp)
{
   Super::EndCursorOver(comp);
}

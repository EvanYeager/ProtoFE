#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/TileActor.h"
#include "Components/StaticMeshComponent.h"

APlayerCharacter::APlayerCharacter() 
{
   Information.Team = ETeam::Player;
   Information.Name = TEXT("Player Character");
}

void APlayerCharacter::SelectCharacter()
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      TMap<FIntPoint, FGridData>* Grid = AGridManager::GetGrid(GetWorld());
      PlayerController->FocusCharacter(this);
      TArray<FIntPoint> RedTiles;
      TArray<AProtoFECharacter*> Chars;
      TArray<FIntPoint> TilesInRange = PlayerController->Pathfinder->BreadthSearch(this, RedTiles, Chars);
      
      for (auto& Tile : TilesInRange)
      {
         Grid->Find(Tile)->TileActor->Plane->SetVisibility(true);
         Grid->Find(Tile)->TileActor->SetColor(EHighlightColor::BlueHighlight);
      }
      for (auto& Tile : RedTiles)
      {
         FGridData* TileData = Grid->Find(Tile);
         TileData->TileActor->Plane->SetVisibility(true);
         TileData->TileActor->SetColor(EHighlightColor::RedHighlight);
      }
   }
}
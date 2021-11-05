#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/Tile.h"
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
      
      UE_LOG(LogTemp, Warning, TEXT("%i"), TilesInRange.Num());
      for (auto& Tile : TilesInRange)
      {
         Grid->Find(Tile)->Tile->Plane->SetVisibility(true);
         Grid->Find(Tile)->Tile->SetColor(EHighlightColor::BlueHighlight);
      }
      for (auto& Tile : RedTiles)
      {
         FGridData* TileData = Grid->Find(Tile);
         TileData->Tile->Plane->SetVisibility(true);
         TileData->Tile->SetColor(EHighlightColor::RedHighlight);
      }
   }
}
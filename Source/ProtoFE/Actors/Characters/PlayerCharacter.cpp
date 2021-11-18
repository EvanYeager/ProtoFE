#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Tile.h"
#include "Components/GridOccupyComponent.h"

APlayerCharacter::APlayerCharacter() 
{
   Information.Team = ETeam::Player;
   Information.Name = TEXT("Player Character");
}

void APlayerCharacter::CharacterClick()
{
   SelectCharacter();
}

void APlayerCharacter::SelectCharacter()
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      if (!GridOccupyComponent->OccupiedTile) return;
      PlayerController->FocusCharacter(this);
      TArray<UTile*> RedTiles;
      TArray<AProtoFECharacter*> Chars;
      TArray<UTile*> TilesInRange = PlayerController->Pathfinder->BreadthSearch(this, RedTiles, Chars);
      
      for (UTile* Tile : TilesInRange)
      {
         Tile->Data.TileActor->HighlightPlane->SetVisibility(true);
         Tile->Data.TileActor->SetColor(EHighlightColor::BlueHighlight);
      }
      for (UTile* Tile : RedTiles)
      {
         Tile->Data.TileActor->HighlightPlane->SetVisibility(true);
         Tile->Data.TileActor->SetColor(EHighlightColor::RedHighlight);
      }
   }
}
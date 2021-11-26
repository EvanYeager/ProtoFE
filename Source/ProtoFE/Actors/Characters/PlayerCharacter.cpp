#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Tile.h"
#include "Components/GridOccupyComponent.h"
#include "Components/HighlightComponent.h"

APlayerCharacter::APlayerCharacter() 
{
   Information.Team = ETeam::Player;
   Information.Name = TEXT("Player Character");
}

void APlayerCharacter::Select() 
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      if (!GridOccupyComponent->OccupiedTile) return;
      PlayerController->FocusCharacter(this);
      TArray<UTile*> RedTiles;
      TArray<AProtoFECharacter*> Chars;
      MovementArea = PlayerController->Pathfinder->BreadthSearch(this, RedTiles, Chars);

      // make the occupied tile green
      MovementArea.Remove(GridOccupyComponent->OccupiedTile);
      PlayerController->HighlightComponent->AddTileHighlight(GridOccupyComponent->OccupiedTile, EHighlightColor::DefaultHighlight, EHighlightStrength::Normal);
      
      // unhighlight the selected tile, then highlight it again so the layers stay correct (path should be under the highlight)
      PlayerController->HighlightComponent->RemoveTileHighlight(PlayerController->SelectedTile);

      for (UTile* Tile : MovementArea)
      {
         PlayerController->HighlightComponent->AddTileHighlight(Tile, EHighlightColor::BlueHighlight, EHighlightStrength::Normal);
      }
      for (UTile* Tile : RedTiles)
      {
         PlayerController->HighlightComponent->AddTileHighlight(Tile, EHighlightColor::RedHighlight, EHighlightStrength::Normal);
      }
      PlayerController->HighlightComponent->HighlightSelectedTile();
   }
}
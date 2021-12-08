#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Tile.h"
#include "Components/GridOccupyComponent.h"
#include "Components/HighlightComponent.h"
#include "AI/ProtoFEAIController.h"

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

      // PlayerController->FocusCharacter(this);

      PlayerController->SetSelectedActor(this);

      TArray<AProtoFECharacter*> Chars;
      MovementArea = PlayerController->Pathfinder->BreadthSearch(this, AttackRangeTiles, Chars);

      HighlightTiles();
   }
}

void APlayerCharacter::UnSelect() 
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      PlayerController->HighlightComponent->ResetPath();
      ResetTiles();
      PlayerController->SetSelectedActor(nullptr);
   }
}

bool APlayerCharacter::ShouldSelect() 
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      return PlayerController->GetSelectedActor() == nullptr;
   }
   return false;
}

void APlayerCharacter::ExecuteCommand() 
{
   if (AProtoFEAIController* AIController = Cast<AProtoFEAIController>(GetController()))
   {

      AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
      AIController->MoveCharacter(PlayerController->Path);
   }
}

void APlayerCharacter::HighlightTiles() 
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      // make the occupied tile green
      MovementArea.Remove(GridOccupyComponent->OccupiedTile);
      PlayerController->HighlightComponent->AddTileHighlight(GridOccupyComponent->OccupiedTile, EHighlightColor::DefaultHighlight, EHighlightStrength::Normal);
      
      // unhighlight the selected tile, then highlight it again later so the layers stay correct (movement range should be under the highlight)
      PlayerController->HighlightComponent->RemoveTileHighlight(PlayerController->GetSelectedTile());

      for (UTile* Tile : MovementArea)
         PlayerController->HighlightComponent->AddTileHighlight(Tile, EHighlightColor::BlueHighlight, EHighlightStrength::Normal);
      for (UTile* Tile : AttackRangeTiles)
         PlayerController->HighlightComponent->AddTileHighlight(Tile, EHighlightColor::RedHighlight, EHighlightStrength::Normal);

      PlayerController->HighlightComponent->HighlightSelectedTile();
   }
}

void APlayerCharacter::ResetTiles() 
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      for (UTile* Tile : MovementArea)
         PlayerController->HighlightComponent->RemoveTileHighlight(Tile);
      for (UTile* Tile : AttackRangeTiles)
         PlayerController->HighlightComponent->RemoveTileHighlight(Tile);
      PlayerController->HighlightComponent->RemoveTileHighlight(GridOccupyComponent->OccupiedTile);
   }
}
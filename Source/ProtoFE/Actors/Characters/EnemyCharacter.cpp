#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProtoFEPlayerController.h"
#include "Components/Pathfinder.h"
#include "Actors/GridManager.h"
#include "Actors/TileActor.h"
// #include "Components/StaticMeshComponent.h"
#include "Tile.h"
#include "Components/HighlightComponent.h"


AEnemyCharacter::AEnemyCharacter() 
{
   Information.Team = ETeam::Enemy;   
   Information.Name = TEXT("Enemy");
}

void AEnemyCharacter::Select() 
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      if (!IsSelected)
      {
         for (UTile* Tile : MovementArea)
         {
            PlayerController->HighlightComponent->RemoveTileHighlight(Tile);
         }
         PlayerController->AddHighlightedTiles(this);
      }
      else
      {
         PlayerController->RemoveHighlightedTiles(this);
      }
      IsSelected = !IsSelected;
      
   }
}

void AEnemyCharacter::OnCursorOver(UPrimitiveComponent* comp)
{
   Super::OnCursorOver(comp);

   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      // if I decide I want this is I can uncomment it
      // if (PlayerController->GetSelectedCharacter()) return;
      if (!IsSelected)
      {
         BreadthSearch();
         for (UTile* Tile : MovementArea)
         {
            if (PlayerController->EnemyRange.Tiles.Contains(Tile)) continue; // don't highlight tiles that are already highlighted
            PlayerController->HighlightComponent->AddTileHighlight(Tile, EHighlightColor::EnemyRange, EHighlightStrength::Weak);
         }
      }
   }
}

void AEnemyCharacter::EndCursorOver(UPrimitiveComponent* comp)
{
   Super::EndCursorOver(comp);

   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      if (!IsSelected)
      {
         for (UTile* Tile : MovementArea)
         {
            PlayerController->HighlightComponent->RemoveTileHighlight(Tile);
         }
      }

      MovementArea.Empty();
   }
}


void AEnemyCharacter::BreadthSearch()
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      TArray<UTile*> RedTiles;
      TArray<AProtoFECharacter*> Chars;
      MovementArea = PlayerController->Pathfinder->BreadthSearch(this, RedTiles, Chars);
      MovementArea.Append(RedTiles); // include attack range
      FilterMovementTiles();
   }
}

void AEnemyCharacter::FilterMovementTiles() 
{
   if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
   {
      TArray<UTile*> Temp;
      for (UTile* Tile : MovementArea)
      {
         if (PlayerController->EnemyRange.Tiles.Contains(Tile))
            Temp.Add(Tile);
      }
      for(UTile* Tile : Temp)
      {
         MovementArea.Remove(Tile);
      }
   }
}
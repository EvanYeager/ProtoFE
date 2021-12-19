#include "BTTask_SetNewLocation.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tile.h"
#include "ProtoFEPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/TileActor.h"
#include "ProtoFEAIController.h"
#include "ProtoFECharacter.h"
#include "Components/GridOccupyComponent.h"

UBTTask_SetNewLocation::UBTTask_SetNewLocation() 
{
   NodeName = "Set New Location";
}

EBTNodeResult::Type UBTTask_SetNewLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
   Super::ExecuteTask(OwnerComp, NodeMemory);

   TilesMovedInOneGo = 1;
   AProtoFEAIController* AIController = Cast<AProtoFEAIController>(OwnerComp.GetAIOwner());

   int MoveCount = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("TilesMovedTo"));
   int PathLength = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("NumOfTilesInPath"));
   if (MoveCount >= PathLength) // if character reached final tile
   {
      AIController->OnMoveFinished();
      OwnerComp.StopLogic("moving is complete");
      return EBTNodeResult::Failed;
   }
   else
   {
      UTile* TargetTile = FindTargetTile(AIController, MoveCount);

      OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("MoveLocation"), TargetTile->Data.TileActor->GetActorLocation());
      IncrementMoveCount(OwnerComp);
   }
   return EBTNodeResult::Succeeded;

}

void UBTTask_SetNewLocation::IncrementMoveCount(UBehaviorTreeComponent& OwnerComp) 
{
   int initial = OwnerComp.GetBlackboardComponent()->GetValueAsInt(TEXT("TilesMovedTo"));
   OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("TilesMovedTo"), initial + TilesMovedInOneGo);
}

UTile* UBTTask_SetNewLocation::FindTargetTile(AProtoFEAIController* AIController, int MoveCount) 
{
   TArray<UTile*> Path = AIController->Path;
   if (Path.Num() == 1) return Path[0]; // if the path only has one tile the calculations are unnecessary

   AProtoFECharacter* Char = Cast<AProtoFECharacter>(AIController->GetCharacter());
   Path.EmplaceAt(0, Char->GridOccupyComponent->OccupiedTile);
   UTile* TargetTile = Path[MoveCount + 1];
   FIntPoint Coords = TargetTile->Data.Coordinates;
   EPathingDirection Direction = EPathingDirection::Reset;
   for (int i = MoveCount + 1; i < Path.Num(); i++)
   {
      UTile* Tile = Path[i];
      UTile* PreviousTile = Path[i - 1];

      switch (Direction)
      {
      case EPathingDirection::Reset:
         // find out if the tile is horizontal or vertical of the previous one
         if (Tile->Data.Coordinates.X == PreviousTile->Data.Coordinates.X)
         {
            Direction = EPathingDirection::Horizontal;
         }
         else
         {
            Direction = EPathingDirection::Vertical;
         }
         break;

      case EPathingDirection::Horizontal:
         if (Tile->Data.Coordinates.X == PreviousTile->Data.Coordinates.X)
         {
            TargetTile = Tile;
            TilesMovedInOneGo++;
         }
         else
         {
            return TargetTile;
         }
         break;

      case EPathingDirection::Vertical:
         if (Tile->Data.Coordinates.Y == PreviousTile->Data.Coordinates.Y)
         {
            TargetTile = Tile;
            TilesMovedInOneGo++;
         }
         else
         {
            return TargetTile;
         }
         break;
      
      default:
         break;
      }
   }
   return Path[Path.Num() - 1];
}



#include "Focusable.h"
#include "ProtoFEPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CameraControllerComponent.h"

void IFocusable::Focus()
{
   if (AActor* ThisActor = Cast<AActor>(this))
   {
      if (AProtoFEPlayerController* PlayerController = Cast<AProtoFEPlayerController>(UGameplayStatics::GetPlayerController(ThisActor->GetWorld(), 0)))
      {
         PlayerController->CameraController->FocusActor(ThisActor);
      }
   }
}
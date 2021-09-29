#include "ProtoFEPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ProtoFECharacter.h"
#include "Actors/ProtoFECamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"

AProtoFEPlayerController::AProtoFEPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AProtoFEPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void AProtoFEPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// camera functions
	InputComponent->BindAxis("MoveUp", this, &AProtoFEPlayerController::MoveCameraUp);
	InputComponent->BindAxis("MoveRight", this, &AProtoFEPlayerController::MoveCameraRight);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AProtoFEPlayerController::ZoomCameraIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AProtoFEPlayerController::ZoomCameraOut);
	InputComponent->BindAction("Back", IE_Pressed, this, &AProtoFEPlayerController::SetFastSpeed);
	InputComponent->BindAction("Back", IE_Released, this, &AProtoFEPlayerController::SetNormalSpeed);

}

void AProtoFEPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	CameraActor = Cast<AProtoFECamera>(GetPawn());
	check(CameraActor);
}

void AProtoFEPlayerController::MoveCameraUp(float Value) 
{
	if (Value == 0.0f) return;
	float Speed = Value * 1.5f * CurrentCameraPanSpeed + (CameraActor->GetCameraBoom()->TargetArmLength / 90 * Value);
	GetPawn()->AddActorWorldOffset(FVector(Speed, 0, 0)); // This probably won't work right when the camera rotates. I might have to use local offset and account for the angle of the camera
}

void AProtoFEPlayerController::MoveCameraRight(float Value) 
{
	if (Value == 0.0f) return;
	float Speed = Value * 1.5f * CurrentCameraPanSpeed + (CameraActor->GetCameraBoom()->TargetArmLength / 90 * Value);
	GetPawn()->AddActorLocalOffset(FVector(0, Speed, 0));
}

void AProtoFEPlayerController::ZoomCameraIn() 
{
	float NewLength = FMath::Clamp<float>(CameraActor->GetCameraBoom()->TargetArmLength - ZoomAmount, CameraZoomLowerBound, CameraZoomUpperBound);
	CameraActor->GetCameraBoom()->TargetArmLength = NewLength;
}

void AProtoFEPlayerController::ZoomCameraOut() 
{
	float NewLength = FMath::Clamp<float>(CameraActor->GetCameraBoom()->TargetArmLength + ZoomAmount, CameraZoomLowerBound, CameraZoomUpperBound);
	CameraActor->GetCameraBoom()->TargetArmLength = NewLength;	
}

void AProtoFEPlayerController::SetFastSpeed() 
{
	CurrentCameraPanSpeed = FastCameraPanMultiplier;
}

void AProtoFEPlayerController::SetNormalSpeed() 
{
	CurrentCameraPanSpeed = NormalCameraPanMultiplier;
}

// void AProtoFEPlayerController::MoveToMouseCursor()
// {
// 	// Trace to see what is under the mouse cursor
// 	FHitResult Hit;
// 	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

// 	if (Hit.bBlockingHit)
// 	{
// 		// We hit something, move there
// 		SetNewMoveDestination(Hit.ImpactPoint);
// 	}
// }

// void AProtoFEPlayerController::SetNewMoveDestination(const FVector DestLocation)
// {
// 	APawn* const MyPawn = GetPawn();
// 	if (MyPawn)
// 	{
// 		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

// 		// We need to issue move command only if far enough in order for walk animation to play correctly
// 		if ((Distance > 120.0f))
// 		{
// 			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
// 		}
// 	}
// }

// void AProtoFEPlayerController::OnSetDestinationPressed()
// {
// 	// set flag to keep updating destination until released
// 	bMoveToMouseCursor = true;
// }

// void AProtoFEPlayerController::OnSetDestinationReleased()
// {
// 	// clear flag to indicate we should stop updating the destination
// 	bMoveToMouseCursor = false;
// }




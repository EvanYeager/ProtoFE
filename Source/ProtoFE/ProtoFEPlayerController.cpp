#include "ProtoFEPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ProtoFECharacter.h"
#include "Actors/ProtoFECamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Actors/GridManager.h"
#include "Actors/Tile.h"
#include "UserWidget.h"

AProtoFEPlayerController::AProtoFEPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableMouseOverEvents = true;
}



void AProtoFEPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	HighlightTile();
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

UUserWidget* AProtoFEPlayerController::DisplayWidget(TSubclassOf<UUserWidget> WidgetClass) 
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(this, WidgetClass);
	Widget->AddToViewport();
	return Widget;
}

void AProtoFEPlayerController::RemoveWidget(UUserWidget* Widget) 
{
	if (!Widget) return;
	Widget->RemoveFromParent();
}

void AProtoFEPlayerController::MoveCameraUp(float Value) 
{
	if (Value == 0.0f) return;
	float Speed = (Value * 2.0f + (CameraActor->GetCameraBoom()->TargetArmLength / 90 * Value)) * CurrentCameraPanSpeed;
	// I have to jump through hoops to move the camera up locally because of the camera's rotation on the pitch
	float Pitch = FMath::Abs(CameraActor->GetCameraBoom()->GetTargetRotation().Pitch);
	float PercentZ = Pitch / 90;
	float PercentX = 1 - PercentZ;
	GetPawn()->AddActorLocalOffset(FVector(PercentX * Speed, 0, PercentZ * Speed));
}

void AProtoFEPlayerController::MoveCameraRight(float Value) 
{
	if (Value == 0.0f) return;
	float Speed = (Value * 2.0f + (CameraActor->GetCameraBoom()->TargetArmLength / 90 * Value)) * CurrentCameraPanSpeed;
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

void AProtoFEPlayerController::HighlightTile() 
{
	// unhighlight previously selected tile, that way if cursor goes off the grid the previous tile will not stay highlighted
	if (SelectedTile)
		SelectedTile->Plane->SetVisibility(false);

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit))
	{
		if (ATile* Tile = Cast<ATile>(Hit.GetActor()))
		{
			SelectedTile = Tile;
			Tile->Plane->SetVisibility(true);
		}
	}
	else SelectedTile = nullptr;
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




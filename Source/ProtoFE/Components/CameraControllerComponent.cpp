#include "CameraControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/ProtoFECamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProtoFECharacter.h"
#include "Interfaces/Focusable.h"

// Sets default values for this component's properties
UCameraControllerComponent::UCameraControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCameraControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	CameraActor = Cast<AProtoFECamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	check(CameraActor);
	
}


// Called every frame
void UCameraControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsFocused && FocusedActor.GetObject())
	{
		AActor* focusedActor = Cast<AActor>(FocusedActor.GetObject());
		FocusLocation(focusedActor->GetActorLocation());
	}
}

void UCameraControllerComponent::MoveCameraUp(float Value)
{
	if (Value == 0.0f) return;

	float Speed = CalculateMoveSpeed(Value);
	// I have to jump through hoops to move the camera up locally because of the camera's rotation on the pitch
	float Pitch = FMath::Abs(CameraActor->GetCameraBoom()->GetTargetRotation().Pitch);
	float PercentZ = Pitch / 90;
	float PercentX = 1 - PercentZ;
	CameraActor->AddActorLocalOffset(FVector(PercentX * Speed, 0, PercentZ * Speed));
	IsFocused = false;
}

void UCameraControllerComponent::MoveCameraRight(float Value)
{
	if (Value == 0.0f) return;

	float Speed = CalculateMoveSpeed(Value);
	CameraActor->AddActorLocalOffset(FVector(0, Speed, 0));
	IsFocused = false;
}

void UCameraControllerComponent::ZoomCameraIn()
{
	float NewLength = FMath::Clamp<float>(CameraActor->GetCameraBoom()->TargetArmLength - ZoomAmount, CameraZoomLowerBound, CameraZoomUpperBound);
	CameraActor->GetCameraBoom()->TargetArmLength = NewLength;		
}

void UCameraControllerComponent::ZoomCameraOut()
{
	float NewLength = FMath::Clamp<float>(CameraActor->GetCameraBoom()->TargetArmLength + ZoomAmount, CameraZoomLowerBound, CameraZoomUpperBound);
	CameraActor->GetCameraBoom()->TargetArmLength = NewLength;	
}

void UCameraControllerComponent::SetFastSpeed()
{
	CurrentCameraPanSpeed = FastCameraPanMultiplier;
}

void UCameraControllerComponent::SetNormalSpeed()
{
	CurrentCameraPanSpeed = NormalCameraPanMultiplier;
}

void UCameraControllerComponent::FocusLocation(FVector Location)
{
	CameraActor->SetActorLocation(FVector(Location.X, Location.Y, CameraActor->GetActorLocation().Z));
}

void UCameraControllerComponent::FocusActor(TScriptInterface<IFocusable> FocusableActor)
{
	IsFocused = true;
	FocusedActor = FocusableActor;
	AActor* TheActor = Cast<AActor>(FocusableActor.GetObject());
	FocusLocation(TheActor->GetActorLocation());
}

void UCameraControllerComponent::SetNormalCameraPanSpeed(float Speed)
{
	NormalCameraPanMultiplier = Speed;
	CurrentCameraPanSpeed = Speed;
}

void UCameraControllerComponent::SetFastCameraPanSpeed(float Speed)
{
	FastCameraPanMultiplier = Speed;
}

float UCameraControllerComponent::CalculateMoveSpeed(float Value)
{
	return (Value * 2.0f + (CameraActor->GetCameraBoom()->TargetArmLength / 90 * Value)) * CurrentCameraPanSpeed;
}

void UCameraControllerComponent::RotateCamera(float Value) 
{
	CameraActor->AddActorWorldRotation(FRotator(0, Value * 2, 0));
}
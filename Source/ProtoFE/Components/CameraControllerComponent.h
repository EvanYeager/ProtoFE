#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraControllerComponent.generated.h"

class AProtoFECamera;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOFE_API UCameraControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraControllerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float NormalCameraPanMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float FastCameraPanMultiplier = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float ZoomAmount = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float CameraZoomLowerBound = 500.0f; // The camera cannot get closer than this distance from the ground
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float CameraZoomUpperBound = 1400.0f; // The camera cannot get further away than this distance from the ground

	void MoveCameraUp(float Value);
	void MoveCameraRight(float Value);
	void ZoomCameraIn();
	void ZoomCameraOut();
	void SetFastSpeed();
	void SetNormalSpeed();
	
	void FocusLocation(FVector Location);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	AProtoFECamera* CameraActor;
	
	float CurrentCameraPanSpeed = NormalCameraPanMultiplier;

	float CalculateMoveSpeed(float Value);
	
};

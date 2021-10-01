#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtoFEPlayerController.generated.h"

class AProtoFECamera;
class ATile;

UCLASS()
class AProtoFEPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProtoFEPlayerController();

	// Camera settings
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



protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	bool bMoveToMouseCursor = true;

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	AProtoFECamera* CameraActor;
	float CurrentCameraPanSpeed = NormalCameraPanMultiplier;

	void MoveCameraUp(float Value);
	void MoveCameraRight(float Value);
	void ZoomCameraIn();
	void ZoomCameraOut();
	void SetFastSpeed();
	void SetNormalSpeed();


	UPROPERTY()
	/** current tile under cursor. May be null. */
	ATile* SelectedTile;

	void HighlightTile();
	
};



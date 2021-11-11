#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtoFEPlayerController.generated.h"

class UCameraControllerComponent;
class ATileActor;
class APlayerCharacter;
class AGridManager;
class UPathfinder;

UCLASS()
class AProtoFEPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProtoFEPlayerController();

	UPROPERTY()
	AGridManager* GridManager;
	
	UUserWidget* DisplayWidget(TSubclassOf<UUserWidget> WidgetClass);
	void RemoveWidget(UUserWidget* Widget);

	void FocusCharacter(APlayerCharacter* Char);

	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetSelectedCharacter();

	UPathfinder* Pathfinder;



protected:
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UCameraControllerComponent* CameraController;

	UPROPERTY()
	/** current tile under cursor. May be null. */
	ATileActor* SelectedTile;

	/** player character that is currently selected, if there is one. May be null. */
	APlayerCharacter* SelectedCharacter = nullptr;

	void HighlightTile();
	void Click();

	void SetSelectedCharacter(APlayerCharacter* SelectedChar);
	
};



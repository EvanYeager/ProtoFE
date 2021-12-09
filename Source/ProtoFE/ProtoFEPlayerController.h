#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtoFEPlayerController.generated.h"

class UCameraControllerComponent;
class ATileActor;
class APlayerCharacter;
class AGridManager;
class UPathfinder;
class UHighlightComponent;
class UTile;
class AEnemyCharacter;
class AProtoFECharacter;
class ISelectable;

USTRUCT(BlueprintType)
struct FEnemyRange
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemyCharacter*> Characters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTile*> Tiles;
	
};

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

	/** Adds an enemy character for highlighted enemy range */
	void AddHighlightedTiles(AEnemyCharacter* Char);
	/** Removes an enemy character from highlighted tiles */
	void RemoveHighlightedTiles(AEnemyCharacter* Char);
	/** removes all highlighted tiles */
	void RemoveHighlightedTiles();

	UFUNCTION()
	UTile* GetSelectedTile();

	UFUNCTION()
	void SetSelectedActor(TScriptInterface<ISelectable> SelectedChar);
	UFUNCTION()
	TScriptInterface<ISelectable> GetSelectedActor();

	UPathfinder* Pathfinder;
	UHighlightComponent* HighlightComponent;

	FEnemyRange EnemyRange;

	/** the selected tile from last frame. May be null. */
	UTile* PreviousTile;
	
	TArray<UTile*> Path;
	
protected:
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UCameraControllerComponent* CameraController;

	UPROPERTY()
	/** current tile under cursor. May be null. */
	UTile* SelectedTile;

	/** actor that is currently selected, if there is one. May be null. */
	TScriptInterface<ISelectable> SelectedActor = nullptr;

	void HighlightTile();

	/** makes the tiles invisible and empties them from the array */
	void ResetEnemyTiles(TArray<UTile*> Tiles);

	bool ShouldPathfind();
	
	void OnLeftClick();
	void OnRightClick();
	void Undo();
	void HandleCameraRotate(float Value);
};



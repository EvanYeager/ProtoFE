#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProtoFEPlayerController.generated.h"

class UCameraControllerComponent;
class ATileActor;
class APlayerCharacter;
class AGridManager;
class UPathfinder;
class UTile;
class AEnemyCharacter;

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

	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetSelectedCharacter();

	UPathfinder* Pathfinder;

	FEnemyRange EnemyRange;

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

	/** makes the tiles invisible and empties them from the array */
	void ResetEnemyTiles(TArray<UTile*> Tiles);
	
};



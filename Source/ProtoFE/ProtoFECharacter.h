#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tile.h"
#include "Interfaces/GridOccupy.h"
#include "Interfaces/Selectable.h"
#include "Interfaces/Hoverable.h"
#include "Interfaces/Damageable.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "ProtoFECharacter.generated.h"

class USnapToGrid;
class UGridOccupyComponent;
class UStatsWindowParent;
class UToolTipParent;
class UHealthBarParent;
class UUserWidget;
class UWidgetComponent;
class UItem;
class UInventoryComponent;
class UAbility;
class UMyGameplayAbility;
class UMyAbilitySystemComponent;
class UMyAttributeSet;
class UAbilityComponent;

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_USTRUCT_BODY()

	FCharacterStats() {}

	FCharacterStats(int8 maxHealth, int8 strength, int8 magic, int8 skill, int8 speed, int8 luck, int8 defense, int8 resistance, int8 level)
	{
		MaxHealth = maxHealth;
		CurrentHealth = maxHealth;
		Strength = strength;
		Magic = magic;
		Skill = skill;
		Speed = speed;
		Luck = luck;
		Defense = defense;
		Resistance = resistance;
		Level = level;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHealth = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealth = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Strength = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Magic = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Skill = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Speed = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Luck = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Defense = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Resistance = 2;

	/* TODO move level and exp into FCharacterInfo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Experience = 0;
};


UENUM(BlueprintType)
enum class ETeam : uint8
{
	Player UMETA(DisplayName="Player"),
	Enemy UMETA(DisplayName="Enemy"),
	Ally UMETA(DisplayName="Ally"),
	Other UMETA(DisplayName="Other")
};


USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

	FCharacterInfo() {}

	FCharacterInfo(FCharacterStats stats)
	{
		Stats = stats;
	}

	FCharacterInfo(FCharacterStats stats, FName name, int movement) : FCharacterInfo(stats)
	{
		Name = name;
		Movement = movement;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterStats Stats = FCharacterStats();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeam Team = ETeam::Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Movement = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = TEXT("Default name");
	/** TODO add more info vars:
	 * portrait/thumbnail
	 */
};

UCLASS(Abstract)
class AProtoFECharacter : public ACharacter, public IGridOccupy, public ISelectable, public IHoverable, public IDamageable
{
	GENERATED_BODY()

public:
	AProtoFECharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	UGridOccupyComponent* GridOccupyComponent;
	UPROPERTY(VisibleAnywhere)
	USnapToGrid* GridSnapComponent;
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterInfo Information = FCharacterInfo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int RemainingMovement = Information.Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// Action Points
	int AP = 4;

	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilityComponent* AbilityComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETerrain, int> TerrainMoveCost;

	UPROPERTY()
	TArray<UTile*> MovementArea;
	
	// UPROPERTY()
	// UMyAttributeSet* Attributes;

	// UPROPERTY(BlueprintReadOnly)
	// TArray<UAbility*> LearnedAbilities;
	// UPROPERTY(BlueprintReadOnly)
	// TMap<int, UAbility*> EquippedAbilities;
	// // starting at 1
	// int NumOfSlots = 9;

	virtual void BreadthSearch();

	virtual void DeleteFromCurrentTile() override;
	virtual void OccupyNewTile(UTile* NewTile) override;

	virtual void Select() override;

	virtual void Wait();
	/** makes the character unable to move again in the same turn */
	virtual void TakeTerminalAction();

	virtual void TakeDamage(int Damage) override;

	// virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PostEditMove(bool bFinished) override;
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnCursorOver(UPrimitiveComponent* Comp) override;
	UFUNCTION()
	virtual void EndCursorOver(UPrimitiveComponent* Comp) override;
	virtual void DisplayStats();
	virtual void RemoveStats();
	virtual void CreateToolTipWindow() override;

	// virtual void PossessedBy(AController* NewController) override;
	// virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// virtual void AddStartupGameplayAbilities();

	
private:

	void RemoveToolTip();
	// void InitAbilities();

	TSubclassOf<UUserWidget> StatsWindowClass;
	UPROPERTY()
	UStatsWindowParent* StatsWindowWidget;

	TSubclassOf<UUserWidget> ToolTipClass;
	UPROPERTY()
	UToolTipParent* ToolTipObj;

	TSubclassOf<UUserWidget> HealthBarClass;
	UPROPERTY()
	UHealthBarParent* HealthBarObj;

	FTimerHandle ToolTipTimer;

	/** A character can only take further actions if active. After terminal actions, this is set to false, and at the beginning of each turn it is set to true */
	bool Active = true;

	// UPROPERTY()
	// UMyAbilitySystemComponent* AbilitySystemComponent;
	//
	// bool AbilitiesInitialized = false;


};


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tile.h"
#include "Interfaces/GridOccupy.h"
#include "Interfaces/Selectable.h"
#include "Interfaces/Hoverable.h"
#include "ProtoFECharacter.generated.h"

class UStatsWindowParent;
class UUserWidget;
class USnapToGrid;
class UGridOccupyComponent;

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
class AProtoFECharacter : public ACharacter, public IGridOccupy, public ISelectable, public IHoverable
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterInfo Information = FCharacterInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETerrain, int> TerrainMoveCost;

	TArray<UTile*> MovementArea;

	virtual void BreadthSearch();

	virtual void DeleteFromCurrentTile() override;
	virtual void OccupyNewTile(UTile* NewTile) override;

	virtual void Select() override;

	// TODO items

protected:
	virtual void BeginPlay() override;
	virtual void PostEditMove(bool bFinished) override;
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnCursorOver(UPrimitiveComponent* comp) override;
	UFUNCTION()
	virtual void EndCursorOver(UPrimitiveComponent* comp) override;
	UFUNCTION()
	virtual void DisplayStats();
	UFUNCTION()
	virtual void RemoveStats();
private:

	TSubclassOf<UUserWidget> StatsWindowClass;
	UStatsWindowParent* StatsWindowWidget;
};


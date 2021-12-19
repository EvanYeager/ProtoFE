

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int Damage = 7;
	UPROPERTY()
	TArray<int> Range = {1};
	UPROPERTY()
	/** expressed as a percent in decimal form (0.0-1.0). */
	float Crit = 0.0f;
	UPROPERTY()
	/** expressed as a percent in decimal form (0.0-1.0). */
	float Hit = 0.75;
	UPROPERTY()
	int Weight = 10;
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Sword UMETA(DisplayName="Sword"),
	Lance UMETA(DisplayName="Lance"),
	Axe UMETA(DisplayName="Axe"),
	Bow UMETA(DisplayName="Bow")
};

UENUM(BlueprintType)
enum class ERank : uint8
{
	E UMETA(DisplayName="E"),
	D UMETA(DisplayName="D"),
	C UMETA(DisplayName="C"),
	B UMETA(DisplayName="B"),
	A UMETA(DisplayName="A"),
	S UMETA(DisplayName="S"),
};

UCLASS()
class PROTOFE_API UWeapon : public UItem
{
	GENERATED_BODY()

public:
	UWeapon();
	
	FORCEINLINE int GetDamage() { return Stats.Damage; }
	FORCEINLINE TArray<int> GetRange() { return Stats.Range; }
	FORCEINLINE int GetWeight() { return Stats.Weight; }
	FORCEINLINE float GetCrit() { return Stats.Crit; }
	FORCEINLINE float GetHit() { return Stats.Hit; }
	FORCEINLINE ERank GetRank() { return Rank; }
	FORCEINLINE EWeaponType GetType() { return WeaponType; }

	FORCEINLINE void SetStats(FWeaponStats NewStats) { Stats = NewStats; }
	

protected:
	FWeaponStats Stats = FWeaponStats();
	
	ERank Rank = ERank::E;
	EWeaponType WeaponType = EWeaponType::Sword;

private:
};

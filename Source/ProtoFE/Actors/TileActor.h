#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileActor.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EHighlightColor : uint8
{
	DefaultHighlight UMETA(DisplayName="DefaultHighlight"),
	BlueHighlight UMETA(DisplayName="BlueHighlight"),
	RedHighlight UMETA(DisplayName="RedHighlight"),
	EnemyRange UMETA(DisplayName="EnemyRange")
};

UENUM(BlueprintType)
enum class EHighlightStrength : uint8
{
	Normal UMETA(DisplayName="Normal"),
	Strong UMETA(DisplayName="Strong"),
	Weak UMETA(DisplayName="Weak")
};

USTRUCT(BlueprintType)
struct FColorLayer
{
	GENERATED_USTRUCT_BODY()

	FColorLayer() {}
	FColorLayer(EHighlightColor C, EHighlightStrength S)
	{
		Color = C;
		Strength = S;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHighlightColor Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHighlightStrength Strength;
};

UCLASS(Blueprintable)
class PROTOFE_API ATileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* HighlightPlane;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* EnemyRangePlane;
	UPROPERTY(VisibleAnywhere)
	UStaticMesh* PlaneMesh;

	// layers of tile colors and strengths
	TArray<FColorLayer> Layers;

	void SetColor(EHighlightColor HighlightColor);
	void SetStrength(EHighlightStrength Strength);

	void SetAsEnemyRange();
	void ResetAsEnemyRange();

	// tile settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Settings")
	float LineThickness = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Settings")
	FVector HighlightColor = FVector(0.7f, 0.7f, 0.7f);

	void AddColorLayer(EHighlightColor Color, EHighlightStrength Strength);
	void RemoveColorLayer();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UMaterialInstanceDynamic* HighlightMaterial;
	
	const FLinearColor DefaultHighlight = FLinearColor(0.187350, 0.641000, 0.318803, 1);
	const FLinearColor BlueHighlight = FLinearColor(.1f, .1f, .8f, 1);
	const FLinearColor RedHighlight = FLinearColor(0.8f, 0.2f, 0, 1);
	const FLinearColor EnemyRange = FLinearColor(1, 0, 0, 1);
	const float NormalStrength = 1.0f;
	const float StrongStrength = 5.0f;
	const float WeakStrength = 0.5f;

	void SwitchToTopLayer();

};

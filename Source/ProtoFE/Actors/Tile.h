#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROTOFE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Plane;
	UPROPERTY(VisibleAnywhere)
	UStaticMesh* PlaneMesh;

	// tile settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Settings")
	float LineThickness = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Settings")
	FVector HighlightColor = FVector(0.7f, 0.7f, 0.7f);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};

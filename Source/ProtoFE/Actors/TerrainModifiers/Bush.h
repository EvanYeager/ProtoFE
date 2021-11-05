

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/TerrainModifiers/TerrainModifier.h"
#include "Bush.generated.h"

class UStaticMeshComponent;
class USnapToGrid;

UCLASS()
class PROTOFE_API ABush : public ATerrainModifier
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABush();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

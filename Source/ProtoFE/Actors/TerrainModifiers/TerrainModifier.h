

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/TerrainModifiers/TerrainMod.h"
#include "Interfaces/GridOccupy.h"
#include "TerrainModifier.generated.h"

class USnapToGrid;
class UGridOccupyComponent;

UCLASS()
class PROTOFE_API ATerrainModifier : public AActor, public ITerrainMod, public IGridOccupy
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainModifier();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UGridOccupyComponent* GridOccupyComponent;

	UPROPERTY(VisibleAnywhere)
	USnapToGrid* GridSnapComponent;

	virtual void OccupyTile(UTile* NewTile) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostEditMove(bool bFinished) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

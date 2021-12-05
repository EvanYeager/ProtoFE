

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/GridSnapInterface.h"
#include "SnapToGrid.generated.h"

class ITerrainMod;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOFE_API USnapToGrid : public UActorComponent, public IGridSnapInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USnapToGrid();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Snap")
	bool SnapToGrid = true;
	
	UFUNCTION(BlueprintCallable)
	void SnapToClosestTile();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
};

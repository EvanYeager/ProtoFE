#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/TileActor.h"
#include "HighlightComponent.generated.h"

class AProtoFEPlayerController;
class UTile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOFE_API UHighlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHighlightComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HighlightSelectedTile();
	void HighlightPath();
	void ResetPath();
	// adds a highlight layer on top
	void AddTileHighlight(UTile* Tile, EHighlightColor Color, EHighlightStrength Strength);
	// removes the top highlight layer
	void RemoveTileHighlight(UTile* Tile);
	// puts a highlight layer at the specified index (0-based)
	void EmplaceHighlightLayer(UTile* Tile, int level, EHighlightColor Color, EHighlightStrength Strength);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AProtoFEPlayerController* Owner;	
};

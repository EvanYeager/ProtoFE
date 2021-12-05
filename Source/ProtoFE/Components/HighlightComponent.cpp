#include "HighlightComponent.h"
#include "ProtoFEPlayerController.h"
#include "Tile.h"
#include "Components/StaticMeshComponent.h"
#include "Actors/TileActor.h"

UHighlightComponent::UHighlightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHighlightComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AProtoFEPlayerController>(GetOwner());
	
}


// Called every frame
void UHighlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHighlightComponent::HighlightSelectedTile()
{
	AddTileHighlight(
		Owner->SelectedTile,
		EHighlightColor::DefaultHighlight, 
		Owner->GetSelectedCharacter() ? EHighlightStrength::Strong : EHighlightStrength::Normal
	);	
}

void UHighlightComponent::AddTileHighlight(UTile* Tile, EHighlightColor Color, EHighlightStrength Strength)
{
	Tile->Data.TileActor->AddColorLayer(Color, Strength);
}

void UHighlightComponent::RemoveTileHighlight(UTile* Tile)
{
	Tile->Data.TileActor->RemoveColorLayer();
}

void UHighlightComponent::EmplaceHighlightLayer(UTile* Tile, int level, EHighlightColor Color, EHighlightStrength Strength)
{
	if (level < 0) return;
	Tile->Data.TileActor->Layers.EmplaceAt(level, FColorLayer(Color, Strength));
}

void UHighlightComponent::HighlightPath()
{
	TArray<UTile*> Path = Owner->Path;
	if (Path.Num() == 0) return;

	for (UTile* Tile : Path)
		Tile->Data.TileActor->AddColorLayer(EHighlightColor::BlueHighlight, EHighlightStrength::Strong);
}

void UHighlightComponent::ResetPath()
{
	TArray<UTile*> Path = Owner->Path;
	if (Path.Num() == 0) return;

	for (UTile* Tile : Path)
		Tile->Data.TileActor->RemoveColorLayer();
	Owner->Path.Empty();
}

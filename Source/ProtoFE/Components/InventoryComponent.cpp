#include "InventoryComponent.h"
#include "Items/Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Inventory.Num() > 0)
		UE_LOG(LogTemp, Warning, TEXT("hello"));
	
}

void UInventoryComponent::GiveItem(UItem* Item)
{
	AddToInventory(Item);
}

void UInventoryComponent::GiveItem(TSubclassOf<UItem> ItemClass)
{
	AddToInventory(NewObject<UItem>(this, ItemClass->StaticClass()));	
}

void UInventoryComponent::RemoveItem(UItem* Item)
{
	Inventory.Remove(Item);
}

void UInventoryComponent::SetAsFirst(UItem* Item)
{
	// is there a better way to do this?
	Inventory.Remove(Item);
	Inventory.EmplaceAt(0, Item);
}

bool UInventoryComponent::AddToInventory(UItem* Item)
{
	if (Inventory.Num() < INVENTORY_CAP)
	{
		Inventory.Add(Item);
		return true;
	}
	return false;
}

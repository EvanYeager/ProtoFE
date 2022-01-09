

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InventoryInterface.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOFE_API UInventoryComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UItem*> Inventory;

	/** give item by pointer. Uses an existing item */
	virtual void GiveItem(UItem* Item) override;
	/** give item by class. Constructs a new item */
	virtual void GiveItem(TSubclassOf<UItem> ItemClass) override;
	/** destroy or take item out of inventory */
	virtual void RemoveItem(UItem* Item) override;
	virtual void SetAsFirst(UItem* Item) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	const int INVENTORY_CAP = 6;

	/** @return if the item was successfully put into the inventory */
	bool AddToInventory(UItem* Item);
};

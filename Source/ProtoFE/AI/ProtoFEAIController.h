

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ProtoFEAIController.generated.h"

class UTile;
class AProtoFECharacter;
class UAbility;

UCLASS()
class PROTOFE_API AProtoFEAIController : public AAIController
{
	GENERATED_BODY()

public:
	AProtoFEAIController();
	void MoveCharacter(TArray<UTile*> path);
	void OnMoveFinished();

	// /** adds a new abilty to the controlled character's learned abilities */
	// void AddAbility(UAbility* Ability);
	// /** adds an ability from the learned list to first empty slot in hotbar. 
	//  * @returns if ability was successfully added to a slot 
	// */
	// bool EquipAbility(UAbility* NewAbility);
	// /** adds an ability from the learned list to a certain slot. 
	//  * @returns if ability was successfully added to slot 
	//  */
	// bool EquipAbility(UAbility* NewAbility, int Slot);
	// /** removes an ability from the equipped list */
	// bool UnequipAbility(UAbility* Ability);

	TArray<UTile*> Path;

protected:
	void BeginPlay() override;

private:
	class UBehaviorTree* AIBehavior;

	AProtoFECharacter* Char;
	
};

#pragma once

#include "CoreMinimal.h"
#include "ProtoFECharacter.h"
#include "Interfaces/Commandable.h"
#include "PlayerCharacter.generated.h"

UCLASS(Blueprintable)
class PROTOFE_API APlayerCharacter : public AProtoFECharacter, public ICommandable
{
	GENERATED_BODY()


public:
	APlayerCharacter();
	
	virtual void Select() override;
	virtual void UnSelect() override;
	virtual bool ShouldSelect() override;

	virtual void ExecuteCommand() override;
	
	TArray<UTile*> AttackRangeTiles;

protected:

private:
	void HighlightTiles();
	void ResetTiles();

};

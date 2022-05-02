#pragma once

#include "CoreMinimal.h"
#include "ProtoFECharacter.h"
#include "Interfaces/Commandable.h"
#include "Interfaces/Focusable.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSelect, APlayerCharacter*, PlayerSelected);

UCLASS(Blueprintable)
class PROTOFE_API APlayerCharacter : public AProtoFECharacter, public ICommandable, public IFocusable
{
	GENERATED_BODY()


public:
	APlayerCharacter();
	
	virtual void Select() override;
	virtual void UnSelect() override;
	virtual bool ShouldSelect() override;

	virtual void ExecuteCommand() override;
	
	TArray<UTile*> AttackRangeTiles;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerSelect PlayerSelected;

protected:

private:
	void HighlightTiles();
	void ResetTiles();

};

#pragma once

#include "CoreMinimal.h"
#include "ProtoFECharacter.h"
#include "EnemyCharacter.generated.h"

class UPrimitiveComponent;

UCLASS()
class PROTOFE_API AEnemyCharacter : public AProtoFECharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	virtual void Select() override;
	virtual void UnSelect() override;
	virtual bool ShouldSelect() override;
	virtual bool ShouldUnSelect() override;

	bool IsSelected = false;
	
	virtual void BreadthSearch() override;
protected:
	virtual void OnCursorOver(UPrimitiveComponent* comp) override;
	virtual void EndCursorOver(UPrimitiveComponent* comp) override;

private:
	void FilterMovementTiles();
};

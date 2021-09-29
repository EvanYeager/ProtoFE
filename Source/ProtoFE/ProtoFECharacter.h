// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProtoFECharacter.generated.h"

UCLASS(Blueprintable)
class AProtoFECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProtoFECharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
};


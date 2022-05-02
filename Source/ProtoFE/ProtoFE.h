// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProtoFE, Log, All);

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
   None UMETA(DisplayName="None"),
   Confirm UMETA(DisplayName="Confirm"),
   Cancel UMETA(DisplayName="Cancel")
};
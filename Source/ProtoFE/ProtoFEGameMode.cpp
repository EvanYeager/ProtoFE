// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProtoFEGameMode.h"
#include "ProtoFEPlayerController.h"
#include "ProtoFECharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Actors/ProtoFECamera.h"

AProtoFEGameMode::AProtoFEGameMode()
{
	PlayerControllerClass = AProtoFEPlayerController::StaticClass();
	DefaultPawnClass = AProtoFECamera::StaticClass();
}
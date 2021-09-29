// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProtoFEGameMode.h"
#include "ProtoFEPlayerController.h"
#include "ProtoFECharacter.h"
#include "UObject/ConstructorHelpers.h"

AProtoFEGameMode::AProtoFEGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProtoFEPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
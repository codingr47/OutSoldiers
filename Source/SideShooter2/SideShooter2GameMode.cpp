// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SideShooter2GameMode.h"
#include "SideShooter2Character.h"
#include "MousePlayerController.h"
#include "UObject/ConstructorHelpers.h"

ASideShooter2GameMode::ASideShooter2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = AMousePlayerController::StaticClass();
	
}

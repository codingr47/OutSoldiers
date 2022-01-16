// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MousePlayerController.h"
AMyGameModeBase::AMyGameModeBase() {
	// Failsafe
	PlayerControllerClass = AMousePlayerController::StaticClass();

	// Blueprinted Version, relies on the asset path obtained from the editor
	static ConstructorHelpers::FClassFinder<AMousePlayerController> VictoryPCOb(TEXT("AMousePlayerController'/Script/SideShooter2.MousePlayerController'"));
	if (VictoryPCOb.Class != NULL)
	{
		PlayerControllerClass = VictoryPCOb.Class;
	}
}
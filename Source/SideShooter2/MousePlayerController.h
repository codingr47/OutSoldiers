// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"	
#include "Blueprint/WidgetTree.h"
#include "CrosshairUserWidget.h"
#include "MousePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIDESHOOTER2_API AMousePlayerController : public APlayerController
{
	GENERATED_BODY()
	
		AMousePlayerController(const FObjectInitializer& ObjectInitializer);
public:
	void BeginPlay();
	void OnXChange(float x);
	void OnYChange(float y);
	void OnLeftClick(float val);
	void UpdateCursor();
	void MoveRight(float Value);
	void Tick(float deltaTime);
	void CenterViewportCursor();
	void UpdateDirection(float x, float y);
	void TickTimer();
private: 
	APawn* controlledCharacter;
	float absoluteDifference(float val, float val2);
	void middlewareCharacterDirection();
	FVector2D getGameViewportSize();
	TSubclassOf<class UUserWidget> tennisWidgetClass;
	TArray<UCrosshairUserWidget*> crosshairs;
	float globalX;
	float globalY;
	float vWidth;
	float vHeight;
	float xDirection;
	float yDirection;
	bool isPlayerMoving;
	bool isCharacterFacingRight;
	FTimerHandle timer;
	const float EPSILON_MOUSE_OVER = 0.0005F;
	const float MOUSE_PLAYER_DIFFERENCE_DELTA = 150.0f;
	const float DELTA_DIFF_X = 20.0f;
	const float MOUSE_SPEED = 5.0f;
	const float SECONDS_SPEED = 2.0f;
	
};

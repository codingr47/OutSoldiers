// Fill out your copyright notice in the Description page of Project Settings.


#include "MousePlayerController.h"
#include "Components/CanvasPanelSlot.h"

AMousePlayerController::AMousePlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	this->bShowMouseCursor = false;
	this->bEnableClickEvents = true;
	this->bEnableMouseOverEvents = true;
	static ConstructorHelpers::FClassFinder<UUserWidget> widgetDefaultCrosshairFinder(TEXT("/Game/UI/tennisCrosshair"));
	tennisWidgetClass = widgetDefaultCrosshairFinder.Class;
}

void AMousePlayerController::UpdateDirection(float x, float y) {
	if (0.0f != x) xDirection = x > 0 ? 1.0f : -1.0f;
	if (0.0f != y) yDirection = y > 0 ? 1.0f : -1.0f;
}

void AMousePlayerController::CenterViewportCursor() 
{
	const ULocalPlayer* LocalPlayer = this->GetLocalPlayer();
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
		if (Viewport)
		{
			FVector2D ViewportSize;
			LocalPlayer->ViewportClient->GetViewportSize(ViewportSize);
			const int32 X = static_cast<int32>(ViewportSize.X * 0.5f);
			const int32 Y = static_cast<int32>(ViewportSize.Y * 0.5f);

			Viewport->SetMouse(X, Y);
		}
	}
}

void AMousePlayerController::Tick(float deltaTime) {
	GetMousePosition(this->globalX, this->globalY);
	//UE_LOG(LogTemp, Warning, TEXT("x: %f, y: %f"), this->globalX, this->globalY);
	UpdateCursor();
}

void AMousePlayerController::BeginPlay() {
	Super::BeginPlay();
	this->globalX = -1;
	this->globalY = -1;
	this->xDirection = 1.0f;
	this->yDirection = -1.0f;
	if (nullptr != InputComponent) {
		InputComponent->BindAxis("MouseX", this, &AMousePlayerController::OnXChange);
		InputComponent->BindAxis("MouseY", this, &AMousePlayerController::OnYChange);
		InputComponent->BindAxis("MouseClickLeft", this, &AMousePlayerController::OnLeftClick);
		InputComponent->BindAxis("MoveRight", this, &AMousePlayerController::MoveRight);
	}	
	AddYawInput(0.1);
	AddPitchInput(0.1);
	UCrosshairUserWidget * widgetTennis = CreateWidget<UCrosshairUserWidget>(this, tennisWidgetClass);
	crosshairs.Add(widgetTennis);
	widgetTennis->AddToPlayerScreen();
	UpdateCursor();
	controlledCharacter = GetPawn();
	isCharacterFacingRight = true;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &AMousePlayerController::TickTimer, SECONDS_SPEED);
}

float AMousePlayerController::absoluteDifference(float val, float val2) {
	return (FMath::Abs(val - val2));
}

void AMousePlayerController::OnXChange(float diffX) {
	FVector2D vSize = getGameViewportSize();
	this->vWidth = vSize.X;
	this->vHeight = vSize.Y;
	if (this->globalX > vWidth - 30.0f) this->globalX = vWidth - 30.0f;
	if (0 > this->globalX) this->globalX = 0.0f;
	//UE_LOG(LogTemp, Warning, TEXT("diffSign: %f, diffX: %f, width:%f, x:%f, y:%f"), diffSign, diffX, this->vWidth, this->globalX, this->globalY);
	middlewareCharacterDirection();
	/*FVector2D vSize = getGameViewportSize();
	this->vWidth = vSize.X;
	this->vHeight = vSize.Y;
	float diffSign = 1.0f;
	if (absoluteDifference(diffX, 0) > EPSILON_MOUSE_OVER) {
		if (this->globalX < 0)  GetMousePosition(this->globalX, this->globalY);
		else {
			UpdateDirection(diffX, 0.0f);
			diffSign = xDirection;// < diffX ? 1.0f : -1.0f;
			float totalDiff = diffSign * MOUSE_SPEED;
			this->globalX += totalDiff;
			//CenterViewportCursor(diffX, 0.0f);
		}
		if (this->globalX > vWidth - 30.0f) this->globalX = vWidth - 30.0f;
		if (0 > this->globalX) this->globalX = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("diffSign: %f, diffX: %f, width:%f, x:%f, y:%f"), diffSign, diffX, this->vWidth, this->globalX, this->globalY);
		UpdateCursor();
		middlewareCharacterDirection();
	}*/
}
void AMousePlayerController::OnYChange(float diffY) {
	/*if (absoluteDifference(diffY, 0) > EPSILON_MOUSE_OVER) {
		float diffSign;
		if (this->globalY < 0)  GetMousePosition(this->globalX, this->globalY);
		else {
			UpdateDirection(0.0f, diffY);
			diffSign = yDirection;
			float totalDiff = diffSign * -1.0f * MOUSE_SPEED;
			this->globalY += totalDiff;
			//CenterViewportCursor(0.0f, diffY);
		}
		if (this->globalY > vHeight) this->globalY = vHeight;
		if (0 > this->globalY) this->globalY = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("diffSign: %f, diffX: %f, height:%f, x:%f, y:%f"), diffSign, diffY, this->vHeight, this->globalX, this->globalY);
		UpdateCursor();
	}*/
}
void AMousePlayerController::OnLeftClick(float val) {
	if (0.0f != val) {
		UpdateCursor();
	}
}
void AMousePlayerController::MoveRight(float value)
{
	bool lastIsPlayerMoving = this->isPlayerMoving;
	if (isPlayerMoving && 0.0F == value) this->isPlayerMoving = false;
	else if (!isPlayerMoving && 0.0F != value) this->isPlayerMoving = true;
	if (0 > value && this->globalX > this->vWidth / 2 && !lastIsPlayerMoving) {
		this->SetMouseLocation((int)(vWidth / 2 - MOUSE_PLAYER_DIFFERENCE_DELTA), (int)vHeight / 2);
		UpdateCursor();
	}
	if (0 < value && this->globalX < this->vWidth / 2 && !lastIsPlayerMoving) {
		this->SetMouseLocation((int)(vWidth / 2 + MOUSE_PLAYER_DIFFERENCE_DELTA), (int)vHeight / 2);
		UpdateCursor();
	}
	controlledCharacter->AddMovementInput(FVector(0.f, -1.f, 0.f), value);
}


void AMousePlayerController::UpdateCursor() {
	UUserWidget* currentWidget = crosshairs[0];
	//UE_LOG(LogTemp, Warning, TEXT("x:%f, y:%f"), this->globalX, this->globalY);
	currentWidget->SetPositionInViewport(FVector2D(globalX, globalY));
	//SetMouseLocation((int)this->globalX, (int)this->globalY);
}

void AMousePlayerController::middlewareCharacterDirection() {
	//UE_LOG(LogTemp, Warning, TEXT("controlledCharacter: %s"), controlledCharacter);
	if (NULL != this->controlledCharacter && !isPlayerMoving) {
		if (!isCharacterFacingRight && (this->globalX > this->vWidth / 2)) { // move right
			FRotator oldRotation = controlledCharacter->GetActorRotation();
			FRotator newRotation(oldRotation.Pitch, FVector(0.f, -1.0f, 0.0f).Rotation().Yaw, oldRotation.Roll);
			controlledCharacter->SetActorRotation(newRotation);
			isCharacterFacingRight = !isCharacterFacingRight;
			//GetWorld()->GetTimerManager().SetTimer(timer, this, &AMousePlayerController::TickTimer, SECONDS_SPEED);
		}
		else if (isCharacterFacingRight && (this->globalX < this->vWidth / 2)) { // move left
			FRotator oldRotation = controlledCharacter->GetActorRotation();
			FRotator newRotation(oldRotation.Pitch, FVector(0.f, 1.0f, 0.0f).Rotation().Yaw, oldRotation.Roll);
			controlledCharacter->SetActorRotation(newRotation);
			isCharacterFacingRight = !isCharacterFacingRight;
			//GetWorld()->GetTimerManager().SetTimer(timer, this, &AMousePlayerController::TickTimer, SECONDS_SPEED);
		}
	}
}

FVector2D AMousePlayerController::getGameViewportSize()
{
	UCrosshairUserWidget* widget = crosshairs[0];
	return widget->GetDimensions();
}
void AMousePlayerController::TickTimer() {
	//bShowMouseCursor = false;
	/*
	SetInputMode(FInputModeGameAndUI());
	CenterViewportCursor();
	SetInputMode(FInputModeGameOnly());
	GetWorld()->GetTimerManager().SetTimer(timer, this, &AMousePlayerController::TickTimer, SECONDS_SPEED);
	*/
}
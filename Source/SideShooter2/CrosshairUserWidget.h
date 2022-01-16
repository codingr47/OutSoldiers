// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIDESHOOTER2_API UCrosshairUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetDimensions(FVector2D dimensions);
	UFUNCTION(BlueprintCallable)
	FVector2D GetDimensions();
private:
	FVector2D mDimensions;
};

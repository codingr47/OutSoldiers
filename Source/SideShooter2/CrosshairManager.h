// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"	
#include "Blueprint/WidgetTree.h"
#include "CrosshairManager.generated.h"

/**
 * 
 */
UCLASS()
class UCrosshairManager : public  UObject
{
	GENERATED_BODY()
public:
	UCrosshairManager();
private:
	TSubclassOf<class UUserWidget> tennisWidgetClass;
	TArray<UUserWidget*> crosshairs;
};

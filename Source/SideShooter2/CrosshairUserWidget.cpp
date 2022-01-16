// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairUserWidget.h"

void UCrosshairUserWidget::SetDimensions(FVector2D dimensions) {
	this->mDimensions = dimensions;
}
FVector2D UCrosshairUserWidget::GetDimensions() { 
	return this->mDimensions;
}
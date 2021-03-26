// Fill out your copyright notice in the Description page of Project Settings.


#include "LumiPlayerController.h"

void ALumiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}

void ALumiPlayerController::SetMouseVisible(bool bShow)
{
	bShowMouseCursor = bShow;
}
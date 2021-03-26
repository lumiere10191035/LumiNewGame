// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LumiPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API ALumiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void SetMouseVisible(bool bShow);
};

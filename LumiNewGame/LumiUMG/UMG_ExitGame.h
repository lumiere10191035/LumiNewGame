// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_ExitGame.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UUMG_ExitGame : public UUserWidget
{
	GENERATED_BODY()
public:
	UUMG_ExitGame(const FObjectInitializer& obj);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CloseExitGameUI();

	UFUNCTION()
	void ExitGame();
};

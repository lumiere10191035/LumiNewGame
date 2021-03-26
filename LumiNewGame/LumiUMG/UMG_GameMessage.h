// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UMG_GameMessage.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UUMG_GameMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_GameMessage(const FObjectInitializer& obj);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateScoreTxt(int _score);

	UFUNCTION()
	void SetGameTime(float Value);

	UFUNCTION()
	void UpdateTargetScore(int _score);

public:
	int64 GameEndTime;

protected:
	UTextBlock* TxtScore;
	UTextBlock* TxtTime;
};

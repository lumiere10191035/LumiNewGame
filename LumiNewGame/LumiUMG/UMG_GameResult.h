// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_GameResult.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UUMG_GameResult : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_GameResult(const FObjectInitializer& obj);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void InitGameSetUI(int playStatus, int score);

	void SetResultStr(FString str, FLinearColor color);

};

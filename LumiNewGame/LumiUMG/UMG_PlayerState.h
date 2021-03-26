// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UMG_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UUMG_PlayerState : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_PlayerState(const FObjectInitializer& obj);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void LifeChange(int Value);
	UFUNCTION()
	void MagicChange(int Value);
	UFUNCTION()
	void ExpChange(int Value);
	UFUNCTION()
	void UpdateState(int maxHP, int curHP, int maxMP, int curMP, int maxExp, int curExp, int level);

protected:
	void UpdateLifeBar();
	void UpdateMagicBar();
	void UpdateExpBar();
	void UpdateShowLevel();

	void OnBtnTestClicked();

	UProgressBar* LifeBar;
	UProgressBar* MagicBar;
	UProgressBar* ExpBar;
	UTextBlock* TxtLife;
	UTextBlock* TxtMagic;
	UTextBlock* TxtLevel;
	UTextBlock* TxtNextLv;

	int maxLife;
	int curLife;
	int maxMagic;
	int curMagic;
	int level;
	int curExp;
	int nextMaxExp;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UMG_ListInner.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UUMG_ListInner : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_ListInner(const FObjectInitializer& obj);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateSkillIcon();
	UFUNCTION()
	void InitInnerIcon();

	void SetInnerOrder(int _order) { order = _order; }
	void UpdateCountDown();
	void MakeCountDownNum();

	bool GetBConstructOver() { return bConstructOver; }
	void UpdateTimer();

protected:
	UTextBlock* Txt_CountDown;
	UImage* Img_Cover;
	UImage* Img_Lock;
	float bConstructOver;
	float CoolTime;

	int order;

	FTimerHandle skillTimerHandle;
	bool bCoolTime;
};

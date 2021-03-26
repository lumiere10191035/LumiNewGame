// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "UMG_BoostBar.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UUMG_BoostBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_BoostBar(const FObjectInitializer& obj);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateBoostValue(float Value, bool bShow);

protected:
	UProgressBar* BoostBar;
	UCanvasPanel* PanelBar;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_BoostBar.h"
#include "Kismet/GameplayStatics.h"
#include "../LumiNewGameGameModeBase.h"

UUMG_BoostBar::UUMG_BoostBar(const FObjectInitializer& obj) : Super(obj)
{

}

void UUMG_BoostBar::NativeConstruct()
{
	BoostBar = Cast<UProgressBar>(GetWidgetFromName("Progress_Boost"));
	PanelBar = Cast<UCanvasPanel>(GetWidgetFromName("Panel_Bar"));
	if (PanelBar != nullptr)
	{
		PanelBar->SetVisibility(ESlateVisibility::Collapsed);
	}

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->updateBoostDelegate.BindUFunction(this, "UpdateBoostValue");
}

void UUMG_BoostBar::UpdateBoostValue(float Value, bool bShow)
{
	if (PanelBar != nullptr)
	{
		if (!bShow)
		{
			PanelBar->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			PanelBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (BoostBar != nullptr)
			{
				BoostBar->SetPercent(Value);
			}
		}
	}
}
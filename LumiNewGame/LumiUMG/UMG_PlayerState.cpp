// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_PlayerState.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../LumiNewGameGameModeBase.h"

UUMG_PlayerState::UUMG_PlayerState(const FObjectInitializer& obj) : Super(obj)
{
	maxLife = 156.0f;
	curLife = maxLife;
	maxMagic = 300.0f;
	curMagic = maxMagic;
}

void UUMG_PlayerState::NativeConstruct()
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	//Progressbar life
	{
		LifeBar = Cast<UProgressBar>(GetWidgetFromName("Progress_Life"));
		//if (LifeBar != nullptr)
		//{
		//	LifeBar->SetPercent(maxLife == 0 ? 1.f : curLife / (float)maxLife);
		//}

		MagicBar = Cast<UProgressBar>(GetWidgetFromName("Progress_Magic"));
		//if (MagicBar != nullptr)
		//{
		//	MagicBar->SetPercent(maxMagic == 0 ? 1.f : curMagic / (float)maxMagic);
		//}
		ExpBar = Cast<UProgressBar>(GetWidgetFromName("Progress_Exp"));

		TxtLife = Cast<UTextBlock>(GetWidgetFromName("Txt_Life"));
		//if (TxtLife != nullptr)
		//{
		//	FString lifeStr = FString::FromInt((int)curLife) + "/" + FString::FromInt((int)maxLife);
		//	TxtLife->SetText(FText::FromString(lifeStr));
		//}

		TxtMagic = Cast<UTextBlock>(GetWidgetFromName("Txt_Magic"));
		//if (TxtMagic != nullptr)
		//{
		//	FString magicStr = FString::Printf(TEXT("%.0f/%.0f"), curMagic, maxMagic);
		//	TxtMagic->SetText(FText::FromString(magicStr));
		//}
		TxtLevel = Cast<UTextBlock>(GetWidgetFromName("Txt_Lv"));
		TxtNextLv = Cast<UTextBlock>(GetWidgetFromName("Txt_NextExp"));
	}

	//Btn Test
	{
		/*UButton* BtnTest = Cast<UButton>(GetWidgetFromName("Btn_Test"));
		if (BtnTest != nullptr)
		{
			BtnTest->OnClicked.__Internal_AddDynamic(this, &UUMG_PlayerState::OnBtnTestClicked, "OnBtnTestClicked");
		}*/
	}

	//Bind function
	lumiGameMode->lifeDelegate.BindUFunction(this, "LifeChange");
	lumiGameMode->MagicDelegate.BindUFunction(this, "MagicChange");
	lumiGameMode->updateStateDelegate.BindUFunction(this, "UpdateState");
	lumiGameMode->UIExpDelegate.BindUFunction(this, "ExpChange");
	// Execute initial UMG
	lumiGameMode->updateRequest.ExecuteIfBound();
}

void UUMG_PlayerState::OnBtnTestClicked()
{

}

void UUMG_PlayerState::LifeChange(int Value)
{
	curLife = Value;
	UpdateLifeBar();
}

void UUMG_PlayerState::MagicChange(int Value)
{
	curMagic = Value;
	UpdateMagicBar();
}

void UUMG_PlayerState::ExpChange(int Value)
{
	curExp = Value;
	UpdateExpBar();
}

void UUMG_PlayerState::UpdateLifeBar()
{
	if (LifeBar != nullptr)
	{
		LifeBar->SetPercent(maxLife == 0 ? 1.f : curLife / (float)maxLife);
	}

	if (TxtLife != nullptr)
	{
		FString lifeStr = FString::FromInt(curLife) + "/" + FString::FromInt(maxLife);
		TxtLife->SetText(FText::FromString(lifeStr));
	}
}

void UUMG_PlayerState::UpdateMagicBar()
{
	if (MagicBar != nullptr)
	{
		MagicBar->SetPercent(maxMagic == 0 ? 1.f : curMagic / (float)maxMagic);
	}

	if (TxtMagic != nullptr)
	{
		FString magicStr = FString::Printf(TEXT("%d/%d"), curMagic, maxMagic);
		TxtMagic->SetText(FText::FromString(magicStr));
	}
}

void UUMG_PlayerState::UpdateExpBar()
{
	if (ExpBar != nullptr)
	{
		ExpBar->SetPercent(nextMaxExp == 0 ? 1.f : curExp / (float)nextMaxExp);
	}

	FString NextLvStr;
	if (TxtNextLv != nullptr)
	{
		if (curExp >= nextMaxExp)
		{
			NextLvStr = TEXT("NextLv --");
		}
		else
		{
			NextLvStr = FString::Printf(TEXT("Next %dexp"), nextMaxExp - curExp);
		}
		TxtNextLv->SetText(FText::FromString(NextLvStr));
	}
}

void UUMG_PlayerState::UpdateShowLevel()
{
	FString LevelStr = TEXT("Lv ") + FString::FromInt(level);
	if (TxtLevel != nullptr)
	{
		TxtLevel->SetText(FText::FromString(LevelStr));
	}
}

void UUMG_PlayerState::UpdateState(int _maxHP, int _curHP, int _maxMP, int _curMP, int _maxExp, int _curExp, int _level)
{
	maxLife = _maxHP;
	curLife = _curHP;
	maxMagic = _maxMP;
	curMagic = _curMP;
	nextMaxExp = _maxExp;
	curExp = _curExp;
	level = _level;

	UpdateLifeBar();
	UpdateMagicBar();
	UpdateExpBar();
	UpdateShowLevel();
}
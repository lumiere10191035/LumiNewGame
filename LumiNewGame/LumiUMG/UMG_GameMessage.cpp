// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_GameMessage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../LumiNewGameGameModeBase.h"

UUMG_GameMessage::UUMG_GameMessage(const FObjectInitializer& obj) : Super(obj)
{

}

void UUMG_GameMessage::NativeConstruct()
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));

	TxtScore = Cast<UTextBlock>(GetWidgetFromName("Txt_Score"));
	TxtTime = Cast<UTextBlock>(GetWidgetFromName("Txt_Time"));

	FString str = "0";
	if (TxtScore != nullptr)
	{
		TxtScore->SetText(FText::FromString(str));
	}

	lumiGameMode->updateScoreDelegate.BindUFunction(this, "UpdateScoreTxt");
	lumiGameMode->updateTimerDelegate.BindUFunction(this, "SetGameTime");
	lumiGameMode->gameTargetDelegate.BindUFunction(this, "UpdateTargetScore");
}

void UUMG_GameMessage::UpdateScoreTxt(int _score)
{
	if (TxtScore != nullptr)
	{
		TxtScore->SetText(FText::FromString(FString::FromInt(_score)));
	}
}

void UUMG_GameMessage::UpdateTargetScore(int _score)
{
	UTextBlock* Txt_Target = Cast<UTextBlock>(GetWidgetFromName("Txt_Target"));
	if (Txt_Target != nullptr)
	{
		Txt_Target->SetText(FText::FromString(FString::FromInt(_score)));
	}
}

void UUMG_GameMessage::SetGameTime(float _Value)
{
	int h = _Value / 3600.f;
	int m = FMath::Fmod(_Value / 60.f, 60.f);
	int s = FMath::Fmod(_Value, 60.f);
	
	FString showTime = FString::Printf(TEXT("%02d:%02d:%02d"), h, m, s);
	if (TxtTime != nullptr)
	{
		TxtTime->SetText(FText::FromString(showTime));
	}
}
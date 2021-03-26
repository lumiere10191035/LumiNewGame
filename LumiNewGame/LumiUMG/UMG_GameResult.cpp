// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_GameResult.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameStateBase.h"
#include "../LumiUtility.h"
#include "../LumiNewGameGameModeBase.h"

UUMG_GameResult::UUMG_GameResult(const FObjectInitializer& obj) : Super(obj)
{

}

void UUMG_GameResult::NativeConstruct()
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->gameSetDelegate.BindUFunction(this, "InitGameSetUI");

	lumiGameMode->gameResultInitDelegate.ExecuteIfBound();
}

void UUMG_GameResult::InitGameSetUI(int playStatus, int score)
{
	UTextBlock* txtScore = Cast<UTextBlock>(GetWidgetFromName("Txt_Score"));
	FString scoreStr = FString::Printf(TEXT("Score: %d"), score);
	if (txtScore != nullptr) 
	{
		txtScore->SetText(FText::FromString(scoreStr));
	}

	switch ((GAME_SET_STATE)playStatus)
	{
	case GAME_RESULT_WIN:
		SetResultStr(TEXT("YOU WIN!"), FLinearColor(0.16f, 0.05f, 1.f));
		break;
	case GAME_RESULT_TIMEUP:
		SetResultStr(TEXT("TIME UP!"), FLinearColor(1.f, 0.16f, 0.f));
		break;
	case GAME_RESULT_PLAYERDEAD:
		SetResultStr(TEXT("YOU DEAD!"), FLinearColor(1.f, 0.f, 0.f));
		break;
	default:
		break;
	}
}

void UUMG_GameResult::SetResultStr(FString str, FLinearColor color)
{
	UTextBlock* txtResult = Cast<UTextBlock>(GetWidgetFromName("Txt_Result"));
	if (txtResult != nullptr)
	{
		txtResult->SetText(FText::FromString(str));
		txtResult->SetColorAndOpacity(FSlateColor(color));
	}
}

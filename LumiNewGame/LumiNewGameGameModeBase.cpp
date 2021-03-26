// Copyright Epic Games, Inc. All Rights Reserved.
#include "LumiNewGameGameModeBase.h"
#include "LumiPlayerController.h"
#include "ReadImportData.h"
#include "PointNPC.h"

void ALumiNewGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InitGameParam();
	MyWidgetInst = nullptr;
	ChangeMenuWidget(StartingWidgetClass);

	scoreDelegate.BindUFunction(this, "AddGameScore");
	gameResultInitDelegate.BindUFunction(this, "UpdateResultUMG");
}

void ALumiNewGameGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

	if (MyWidgetInst != nullptr)
	{
		MyWidgetInst->RemoveFromViewport();
		MyWidgetInst = nullptr;
	}
}

void ALumiNewGameGameModeBase::OnClickStartBtn()
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	InitGameUMG();
	InitGamePointObj();
}

void ALumiNewGameGameModeBase::InitGameParam()
{
	FLumiGameParam gameParam;
	if (UReadImportData::ImportGameParam(gameParam))
	{
		TargetScore = gameParam.TargetScore;
		GameTime = gameParam.GameMaxTime;

		LumiScore = 0;
		TimeCount = 0.f;
	}
}

void ALumiNewGameGameModeBase::InitGameUMG()
{
	if (MyWidgetInst != nullptr)
	{
		MyWidgetInst->RemoveFromViewport();
		MyWidgetInst = nullptr;
	}

	FString widgetBP = TEXT("WidgetBlueprint'/Game/UMG/Lumi_Status.Lumi_Status_C'");
	FString widgetTime = TEXT("WidgetBlueprint'/Game/UMG/Lumi_Game.Lumi_Game_C'");
	FString widgetBoost = TEXT("WidgetBlueprint'/Game/UMG/Lumi_BoostBar.Lumi_BoostBar_C'");
	FString widgetSkill = TEXT("WidgetBlueprint'/Game/UMG/Lumi_SkillList.Lumi_SkillList_C'");
	CreateNewUserUMG(widgetBP);
	CreateNewUserUMG(widgetTime);
	CreateNewUserUMG(widgetBoost);
	CreateNewUserUMG(widgetSkill);

	//Target Update;
	gameTargetDelegate.ExecuteIfBound(TargetScore);

	FVector startPos = FVector(-10000.f, 0.f, 97.5f);
	LumiCameraActor = GetWorld()->SpawnActor<ALumiCameraThird>(ALumiCameraThird::StaticClass(), startPos, FRotator::ZeroRotator);
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	controller->SetViewTarget(LumiCameraActor);

	//timer
	FDateTime Time = FDateTime::Now();
	int64 Timestamp = Time.ToUnixTimestamp();
	GameEndTime = Timestamp + (int64)GameTime;
	TimeCount = 0.f;
	UpdateTimer();
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ALumiNewGameGameModeBase::UpdateTimer, 1.f, true);
}

void ALumiNewGameGameModeBase::InitGamePointObj()
{
	TArray<FPointObjData> PointDataList;
	TArray<FString> NameList;

	if (UReadImportData::ImportPointObj(PointDataList, NameList))
	{
		// Read PointNPC
		for (auto npcData : PointDataList)
		{
			FVector pos = FVector(npcData.PosX, npcData.PosY, npcData.PosZ);
			FString path = "\"" +  npcData.Path + "\"";

			//check(GEngine != nullptr);
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Create POINT"));

			FActorSpawnParameters defaultParam;
			APointNPC* newNpc = GetWorld()->SpawnActor<APointNPC>(APointNPC::StaticClass(), pos, FRotator::ZeroRotator);
			//newNpc->AttachStaticMesh(path);
			newNpc->UpdateInitData(npcData.Radius, npcData.Size, npcData.Score, npcData.Exp, npcData.Path);
		}
	}
}

void ALumiNewGameGameModeBase::AddGameScore(int _score)
{
	LumiScore += _score;
	updateScoreDelegate.ExecuteIfBound(LumiScore);

	if (LumiScore >= TargetScore)
	{
		GameResultState = GAME_RESULT_WIN;
		ShowResultWidget();
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, gameSetDelegate.IsBound() ? TEXT("Yes") : TEXT("No"));
		//gameSetDelegate.ExecuteIfBound((int)GAME_RESULT_WIN, LumiScore);
	}
}

void ALumiNewGameGameModeBase::UpdateTimer()
{
	FDateTime Time = FDateTime::Now();
	int64 Timestamp = Time.ToUnixTimestamp();
	float RemainTime = (float)(GameEndTime - Timestamp);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Timer:: %.0f"), RemainTime));
	if (TimeCount >= GameTime)
	{
		RemainTime = 0.f;
		// GameSet;
		GameResultState = GAME_RESULT_TIMEUP;
		ShowResultWidget();
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);

		//gameSetDelegate.ExecuteIfBound((int)GAME_RESULT_TIMEUP, LumiScore);
	}
	else
	{
		updateTimerDelegate.ExecuteIfBound(RemainTime);
		TimeCount += 1.0f;
	}
}

void ALumiNewGameGameModeBase::ShowResultWidget()
{
	FString widgetResult = TEXT("WidgetBlueprint'/Game/UMG/Lumi_Result.Lumi_Result_C'");
	CreateNewUserUMG(widgetResult);
}

void ALumiNewGameGameModeBase::UpdateResultUMG()
{
	gameSetDelegate.ExecuteIfBound((int)GameResultState, LumiScore);
}

void ALumiNewGameGameModeBase::ShowExitGameUMG()
{
	if (bGameExitShow)
	{
		closeExitUMGDelegate.ExecuteIfBound();
	}
	else
	{
		SetBGameExitShow(true);

		FString widgetExit = TEXT("WidgetBlueprint'/Game/UMG/Lumi_ExitGame.Lumi_ExitGame_C'");
		CreateNewUserUMG(widgetExit);
	}
}

void ALumiNewGameGameModeBase::SetBGameExitShow(bool b)
{
	bGameExitShow = b;

	if (bGameExitShow)
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	}
	else
	{
		FDateTime Time = FDateTime::Now();
		int64 Timestamp = Time.ToUnixTimestamp();
		GameEndTime = Timestamp + (int64)(GameTime - TimeCount);
		//UpdateTimer();
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ALumiNewGameGameModeBase::UpdateTimer, 1.f, true);
	}
}
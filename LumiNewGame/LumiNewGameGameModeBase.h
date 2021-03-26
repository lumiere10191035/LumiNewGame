// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "LumiImportData.h"
#include "LumiCameraThird.h"
#include "Kismet/GameplayStatics.h"
#include "LumiUtility.h"
#include "LumiNewGameGameModeBase.generated.h"

DECLARE_DELEGATE_OneParam(FLifeChange, int)
DECLARE_DELEGATE_OneParam(FMagicChange, int)
DECLARE_DELEGATE_OneParam(FExpChange, int)
DECLARE_DELEGATE_SevenParams(FStateUpdate, int, int, int, int, int, int, int)
DECLARE_DELEGATE(FNoParamDelegate)
DECLARE_DELEGATE_OneParam(FAddCharaExp, int)
DECLARE_DELEGATE_OneParam(FAddScore, int)
DECLARE_DELEGATE_OneParam(FUpdateScore, int)
DECLARE_DELEGATE_TwoParams(FUpdateBoost, float, bool)
DECLARE_DELEGATE_OneParam(FUpdateTimer, float)
DECLARE_DELEGATE_TwoParams(FGameSet, int, int)
DECLARE_DELEGATE_OneParam(FGameTarget, int)

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API ALumiNewGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void OnClickStartBtn();

	UFUNCTION()
	void InitGameParam();

	UFUNCTION()
	void InitGameUMG();

	UFUNCTION()
	void AddGameScore(int _score);

	UFUNCTION()
	void UpdateTimer();

	UFUNCTION()
	void UpdateResultUMG();

	UFUNCTION()
	void ShowResultWidget();

	UFUNCTION()
	void ShowExitGameUMG();

	void SetBGameExitShow(bool b);
	bool GetBGameExitShow() { return bGameExitShow; }

protected:
	virtual void BeginPlay() override;

	void InitGamePointObj();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY()
	UUserWidget* MyWidgetInst;

	// game score;
	int LumiScore;
	GAME_SET_STATE GameResultState = GAME_RESULT_DEFAULT;

	// game time;
	float GameTime = 120.f;
	int TargetScore = 100;
	int64 GameEndTime;
	float TimeCount = 0.f;
	FTimerHandle timerHandle;

	// game exit
	bool bGameExitShow = false;

public:
	// GameUtility
	bool CreateNewUserUMG(FString path);
	AActor* CreateNewActorByBP(FString path);

	UPROPERTY(VisibleAnywhere)
	ALumiCameraThird* LumiCameraActor;

public:
	FLifeChange lifeDelegate;
	FMagicChange MagicDelegate;
	FExpChange ExpDelegate;
	FStateUpdate updateStateDelegate;
	FNoParamDelegate updateRequest;
	FAddCharaExp charaExpDelegate;
	FAddScore scoreDelegate;
	FUpdateScore updateScoreDelegate;
	FUpdateBoost updateBoostDelegate;
	FUpdateTimer updateTimerDelegate;
	FGameSet gameSetDelegate;
	FGameTarget gameTargetDelegate;
	FNoParamDelegate gameResultInitDelegate;
	FNoParamDelegate closeExitUMGDelegate;
};

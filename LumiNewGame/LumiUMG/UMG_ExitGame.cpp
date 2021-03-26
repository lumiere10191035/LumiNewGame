// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_ExitGame.h"
#include "Components/Button.h"
#include "../LumiNewGameGameModeBase.h"

UUMG_ExitGame::UUMG_ExitGame(const FObjectInitializer& obj) : Super(obj)
{

}

void UUMG_ExitGame::NativeConstruct()
{
	UButton* closeBtn = Cast<UButton>(GetWidgetFromName("Button_Close"));
	UButton* exitBtn = Cast<UButton>(GetWidgetFromName("Button_Exit"));
	if (closeBtn != nullptr)
	{
		closeBtn->OnClicked.__Internal_AddDynamic(this, &UUMG_ExitGame::CloseExitGameUI, "CloseExitGameUI");
	}
	if (exitBtn != nullptr)
	{
		exitBtn->OnClicked.__Internal_AddDynamic(this, &UUMG_ExitGame::ExitGame, "ExitGame");
	}

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->closeExitUMGDelegate.BindUFunction(this, "CloseExitGameUI");
}

void UUMG_ExitGame::CloseExitGameUI()
{
	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->SetBGameExitShow(false);

	RemoveFromViewport();
}

void UUMG_ExitGame::ExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}
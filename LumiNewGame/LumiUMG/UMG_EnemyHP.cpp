// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_EnemyHP.h"
#include "../LumiNewGameGameModeBase.h"

UUMG_EnemyHP::UUMG_EnemyHP(const FObjectInitializer& obj) : Super(obj)
{
	enemyName = TEXT("");
	maxHP = 1;
	curHP = 0;
}

void UUMG_EnemyHP::NativeConstruct()
{
	Txt_HP = Cast<UTextBlock>(GetWidgetFromName("Txt_HP"));
	Txt_Name = Cast<UTextBlock>(GetWidgetFromName("Txt_Name"));
	Bar_HP = Cast<UProgressBar>(GetWidgetFromName("Progress_HP"));

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	//lumiGameMode->closeExitUMGDelegate.BindUFunction(this, "CloseExitGameUI");
}

void UUMG_EnemyHP::InitEnemyData(const FString& _name, const int& _maxHP, const int& _curHP)
{
	maxHP = _maxHP;
	curHP = _curHP;
	enemyName = _name;

	if (Txt_Name != nullptr)
	{
		Txt_Name->SetText(FText::FromString(_name));
	}
	UpdateHPBar(_curHP);
}

void UUMG_EnemyHP::UpdateHPBar(int _curHp)
{
	curHP = _curHp;

	FString hpTxt = FString::Printf(TEXT("%d/%d"), curHP, maxHP);
	if (Txt_HP)
	{
		Txt_HP->SetText(FText::FromString(hpTxt));
	}

	if (maxHP > 0)
	{
		float HpPercent = FMath::Clamp((float)curHP / (float)maxHP, 0.f, 1.f);

		if (Bar_HP)
		{
			Bar_HP->SetPercent(HpPercent);
		}
	}
}
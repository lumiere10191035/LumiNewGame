// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_ListInner.h"
#include "Kismet/GameplayStatics.h"
#include "../LumiCharacter.h"
#include "../LumiNewGameGameModeBase.h"

UUMG_ListInner::UUMG_ListInner(const FObjectInitializer& obj) : Super(obj)
{
	order = -1;
	bCoolTime = false;
}

void UUMG_ListInner::NativeConstruct()
{
	Txt_CountDown = Cast<UTextBlock>(GetWidgetFromName("Txt_CountDown"));
	Img_Cover = Cast<UImage>(GetWidgetFromName("Img_Cover"));
	Img_Lock = Cast<UImage>(GetWidgetFromName("Img_Lock"));

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->updateSkillIconDelegate.BindUFunction(this, "InitInnerIcon");

	if (order > 0)
	{
		lumiGameMode->updateSkillIconDelegate.ExecuteIfBound();
	}
}

void UUMG_ListInner::InitInnerIcon()
{
	if (Txt_CountDown != nullptr)
	{
		Txt_CountDown->SetText(FText::FromString("14"));
	}

	ALumiCharacter* myCharacter = Cast<ALumiCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (myCharacter == nullptr)
		return;
	if (myCharacter->lumiSkillList.Num() < order)
		return;
	LumiSkillStruct lumiSkill = myCharacter->lumiSkillList[order - 1];
	FString iconName = lumiSkill.skillData.IconName;
	FString ImgStr = TEXT("Texture2D'/Game/Textures/") + iconName + TEXT(".") + iconName + "'";
	UImage* Img_Skill = Cast<UImage>(GetWidgetFromName("Img_Skill"));
	UTexture2D* imgTexture = LoadObject<UTexture2D>(NULL, *ImgStr);
	if (imgTexture && Img_Skill)
	{
		Img_Skill->SetBrushFromTexture(imgTexture);
	}
}

void UUMG_ListInner::UpdateSkillIcon()
{
	if (order < 0)
		return;

	ALumiCharacter* myCharacter = Cast<ALumiCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (myCharacter == nullptr)
	{
		return;
	}
	LumiSkillStruct lumiSkill = myCharacter->lumiSkillList[order - 1];

	// skill lock
	if (Img_Lock != nullptr)
	{
		Img_Lock->SetVisibility(lumiSkill.bUnlock ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}
	
	// cool time
	if (lumiSkill.curCoolTime > -1.f || myCharacter->GetCurStaticCoolTimeShow() > 0.f)
	{
		if (lumiSkill.bUnlock)
		{
			float countDown = lumiSkill.curCoolTime > -1.f ?
				lumiSkill.skillData.CoolTime - lumiSkill.curCoolTime : -1.f;
			CoolTime = FMath::Max(countDown, myCharacter->GetCurStaticCoolTimeShow());
			UpdateTimer();
		}
	}
	else
	{
		CoolTime = -1.f;
		if (Txt_CountDown != nullptr && Txt_CountDown->GetVisibility() != ESlateVisibility::Collapsed)
		{
			Txt_CountDown->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (Img_Cover != nullptr && Img_Cover->GetVisibility() != ESlateVisibility::Collapsed)
		{
			Img_Cover->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUMG_ListInner::UpdateCountDown()
{
	if (Txt_CountDown != nullptr)
	{
		if (Txt_CountDown->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
		{
			Txt_CountDown->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		FString NumStr;
		if (CoolTime >= 1.f)
		{
			NumStr = FString::Printf(TEXT("%.0f"), CoolTime);
		}
		else if(CoolTime > 0.f)
		{
			NumStr = FString::Printf(TEXT("%.1f"), CoolTime);
		}
		else {
			UpdateTimer();
		}
		Txt_CountDown->SetText(FText::FromString(NumStr));
	}

	if (CoolTime > 0.f && 
		Img_Cover != nullptr && 
		Img_Cover->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		Img_Cover->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUMG_ListInner::UpdateTimer()
{
	if (CoolTime > 0.f)
	{
		UpdateCountDown();
		bCoolTime = true;
		GetWorld()->GetTimerManager().SetTimer(skillTimerHandle, this, &UUMG_ListInner::MakeCountDownNum, 0.1f, true);
	}
	else
	{
		bCoolTime = false;
		GetWorld()->GetTimerManager().ClearTimer(skillTimerHandle);
	}
}

void UUMG_ListInner::MakeCountDownNum()
{
	CoolTime -= 0.1f;
	UpdateCountDown();
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SkillList.h"
#include "Kismet/GameplayStatics.h"
#include "../LumiNewGameGameModeBase.h"

UUMG_SkillList::UUMG_SkillList(const FObjectInitializer& obj) : Super(obj)
{

}

void UUMG_SkillList::NativeConstruct()
{
	//List_Skill = Cast<UListView>(GetWidgetFromName("List_Skill"));

	InitSkillList();

	ALumiNewGameGameModeBase* lumiGameMode;
	lumiGameMode = Cast<ALumiNewGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	lumiGameMode->updateSkillListDelegate.BindUFunction(this, "UpdateSkillList");
	lumiGameMode->updateSingleIconDelegate.BindUFunction(this, "UpdateSingleIcon");

	//lumiGameMode->updateSkillDelegate.ExecuteIfBound();
}

void UUMG_SkillList::InitSkillList()
{
	skillIconList.Empty();
	for (int i = 0; i < SKILL_MAX_NUM; i++)
	{
		FString iconName = FString::Printf(TEXT("Lumi_SkillIcon_%d"), i + 1);
		UUMG_ListInner* iconInner = Cast<UUMG_ListInner>(GetWidgetFromName(*iconName));
		if (iconInner)
		{
			skillIconList.Emplace(iconInner);
			iconInner->SetInnerOrder(i + 1);
			iconInner->InitInnerIcon();
			iconInner->UpdateSkillIcon();
		}
	}
}

void UUMG_SkillList::UpdateSkillList()
{
	for (auto icon : skillIconList)
	{
		if (icon != nullptr)
		{
			icon->UpdateSkillIcon();
		}
	}
	/*if (List_Skill != nullptr)
	{
		for (int i = 0; i < List_Skill->GetNumItems(); i++)
		{
			UUMG_ListInner* listItem = Cast<UUMG_ListInner>(List_Skill->GetItemAt(i));
			if (listItem != nullptr)
			{
				listItem->UpdateSkillIcon();
			}
		}
	}*/
}

void UUMG_SkillList::UpdateSingleIcon(int _skillId)
{
	/*if (List_Skill == nullptr)
		return;

	if (_skillId > List_Skill->GetNumItems())
		return;
	*/
	if (skillIconList.Num() < _skillId)
		return;
	UUMG_ListInner* listItem = Cast<UUMG_ListInner>(skillIconList[_skillId - 1]);
	if (listItem != nullptr)
	{
		listItem->UpdateSkillIcon();
	}
}


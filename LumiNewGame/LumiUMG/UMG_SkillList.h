// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "UMG_ListInner.h"
#include "UMG_SkillList.generated.h"

#define SKILL_MAX_NUM 4
/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UUMG_SkillList : public UUserWidget
{
	GENERATED_BODY()
public:
	UUMG_SkillList(const FObjectInitializer& obj);
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateSkillList();
	UFUNCTION()
	void UpdateSingleIcon(int _skillId);

	void InitSkillList();

protected:
	TArray<UUMG_ListInner*> skillIconList;
};

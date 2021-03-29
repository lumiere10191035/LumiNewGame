// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UMG_EnemyHP.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UUMG_EnemyHP : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_EnemyHP(const FObjectInitializer& obj);
	virtual void NativeConstruct() override;

	//UFUNCTION()
	//	void CloseExitGameUI();
	void UpdateHPBar(int _curHp);

	void InitEnemyData(const FString& _name, const int& _maxHP, const int& _curHP);
	//UFUNCTION()
	//	void ExitGame();

protected:
	UTextBlock* Txt_Name;
	UTextBlock* Txt_HP;
	UProgressBar* Bar_HP;

	FString enemyName;
	int maxHP;
	int curHP;
};

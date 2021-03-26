// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LUMINEWGAME_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMyUserWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnBtn_TestClick();
};

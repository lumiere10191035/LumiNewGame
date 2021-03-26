// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/Button.h"

UMyUserWidget::UMyUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UMyUserWidget::NativeConstruct()
{
	UButton* btn = Cast<UButton>(GetWidgetFromName("Btn_Test"));
	if (btn != nullptr)
	{
		btn->OnClicked.AddDynamic(this, &UMyUserWidget::OnBtn_TestClick);
	}
}

void UMyUserWidget::OnBtn_TestClick()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("OnClickTestBtn"));
}
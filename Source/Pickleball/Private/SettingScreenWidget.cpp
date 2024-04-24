// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingScreenWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USettingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &USettingScreenWidget::OnExitButtonClicked);
	}
}

void USettingScreenWidget::OnExitButtonClicked()
{
	RemoveFromParent();
	OnSettingsClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeScreenWidget.h"

#include "SettingScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"


class USettingScreenWidget;

void UHomeScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(PlayButton!= nullptr)
	{
		PlayButton->OnClicked.AddDynamic(this,&UHomeScreenWidget::OnPlayButtonClicked);
	}
	if(SettingsButton!= nullptr)
	{
		SettingsButton->OnClicked.AddDynamic(this,&UHomeScreenWidget::OnSettingsButtonClicked);
	}
}

void UHomeScreenWidget::OnPlayButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("Countdown"), GetWorld());
	
}

void UHomeScreenWidget::OnSettingsButtonClicked()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	USettingScreenWidget* SettingScreenWidget = Cast<USettingScreenWidget>(WidgetLoader->LoadWidget(FName("SettingScreen"), GetWorld(),  1));
	SettingScreenWidget->OnSettingsClosed.AddDynamic(this, &UHomeScreenWidget::HandleSettingsClosed);
}

void UHomeScreenWidget::HandleSettingsClosed()
{
	SetVisibility(ESlateVisibility::Visible);
}

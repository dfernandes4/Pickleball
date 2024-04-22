// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeScreenWidget.h"

#include "CollectionWidget.h"
#include "SettingScreenWidget.h"
#include "ShopScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


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
		UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	}
	if(CollectionButton!= nullptr)
	{
		CollectionButton->OnClicked.AddDynamic(this,&UHomeScreenWidget::OnCollectionButtonClicked);
		UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	}
	if(ShopButton!= nullptr)
	{
		ShopButton->OnClicked.AddDynamic(this,&UHomeScreenWidget::OnShopButtonClicked);
		UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	}
}

void UHomeScreenWidget::OnPlayButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("Countdown"), GetWorld());
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	
}

void UHomeScreenWidget::OnSettingsButtonClicked()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	USettingScreenWidget* SettingScreenWidget = Cast<USettingScreenWidget>(WidgetLoader->LoadWidget(FName("SettingScreen"), GetWorld(),  1));
	SettingScreenWidget->OnSettingsClosed.AddDynamic(this, &UHomeScreenWidget::HandleChildClosed);
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UHomeScreenWidget::OnCollectionButtonClicked()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	UCollectionWidget* CollectionWidget = Cast<UCollectionWidget>(WidgetLoader->LoadWidget(FName("CollectionScreen"), GetWorld(),  1));
	CollectionWidget->OnCollectionClosed.AddDynamic(this, &UHomeScreenWidget::HandleChildClosed);
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UHomeScreenWidget::OnShopButtonClicked()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	UShopScreenWidget* ShopScreenWidget = Cast<UShopScreenWidget>(WidgetLoader->LoadWidget(FName("PaddleShopScreen"), GetWorld(),  1));
	ShopScreenWidget->OnShopClosed.AddDynamic(this, &UHomeScreenWidget::HandleChildClosed);
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UHomeScreenWidget::HandleChildClosed()
{
	SetVisibility(ESlateVisibility::Visible);
}

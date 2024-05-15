// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeScreenWidget.h"

#include "CoinShopScreen.h"
#include "CollectionWidget.h"
#include "PlayerPaddle.h"
#include "SettingScreenWidget.h"
#include "ShopScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
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
	}
	if(CollectionButton!= nullptr)
	{
		CollectionButton->OnClicked.AddDynamic(this,&UHomeScreenWidget::OnCollectionButtonClicked);
	}
	if(ShopButton!= nullptr)
	{
		ShopButton->OnClicked.AddDynamic(this,&UHomeScreenWidget::OnShopButtonClicked);
	}
	if(PlusCoinButton != nullptr)
	{
		PlusCoinButton->OnClicked.AddDynamic(this, &UHomeScreenWidget::UHomeScreenWidget::OnPlusCoinClicked);
	}

	APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if(HighScoreAmountTextBlock != nullptr && PlayerPaddle != nullptr)
	{
		HighScoreAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerPaddle->GetHighScore())));
	}
	
	if(CoinAmountTextBlock != nullptr && PlayerPaddle != nullptr)
	{
		CoinAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerPaddle->GetPlayerCoins())));
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

void UHomeScreenWidget::OnPlusCoinClicked()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	UCoinShopScreen* CoinShopScreenWidget = Cast<UCoinShopScreen>(WidgetLoader->LoadWidget(FName("CoinShopScreen"), GetWorld(),  1));
	CoinShopScreenWidget->OnCoinShopClosed.AddDynamic(this, &UHomeScreenWidget::HandleChildClosed);
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UHomeScreenWidget::HandleChildClosed()
{
	SetVisibility(ESlateVisibility::Visible);
}

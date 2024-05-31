// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeScreenWidget.h"

#include "CoinShopScreen.h"
#include "CollectionWidget.h"
#include "MainGamemode.h"
#include "SaveGameInterface.h"
#include "SettingScreenWidget.h"
#include "ShopScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Components/Image.h"
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

	DisplayPlayerValues();
	
	AMainGamemode* MainGamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(MainGamemode)
	{
		MainGamemode->OnGameOver.AddDynamic(this, &UHomeScreenWidget::DisplayPlayerValues);
	}
}

void UHomeScreenWidget::OnPlayButtonClicked()
{
	RemoveFromParent();
	
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
	CollectionWidget->OnPaddleSelected.AddDynamic(this, &UHomeScreenWidget::DisplayPaddles);
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UHomeScreenWidget::DisplayPaddles(UPaddleToCollectWidget* PaddleSelected, UPaddleToCollectWidget* PaddleBefore,
	UPaddleToCollectWidget* PaddleAfter)
{
	TTuple<UObject*, const FVector2D&> PaddleBeforeImageInfo = PaddleBefore->GetPaddleImageInfo();
	TTuple<UObject*, const FVector2D&> PaddleSelectedImageInfo = PaddleSelected->GetPaddleImageInfo();
	TTuple<UObject*, const FVector2D&> PaddleAfterImageInfo = PaddleAfter->GetPaddleImageInfo();

	PaddleLeft->SetBrushFromTexture(Cast<UTexture2D>(PaddleBeforeImageInfo.Key));
	PaddleLeft->SetDesiredSizeOverride(PaddleBeforeImageInfo.Value * 2);

	PaddleLeft->SetBrushFromTexture(Cast<UTexture2D>(PaddleSelectedImageInfo.Key));
	PaddleLeft->SetDesiredSizeOverride(PaddleSelectedImageInfo.Value * 2);

	PaddleLeft->SetBrushFromTexture(Cast<UTexture2D>(PaddleAfterImageInfo.Key));
	PaddleLeft->SetDesiredSizeOverride(PaddleAfterImageInfo.Value * 2);
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

void UHomeScreenWidget::DisplayPlayerValues()
{
	ISaveGameInterface* SaveGameInterface = Cast<ISaveGameInterface>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (SaveGameInterface)
	{
		FPlayerData PlayerData = SaveGameInterface->GetSaveGamePlayerData();

		if(HighScoreAmountTextBlock != nullptr )
		{
			HighScoreAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerData.PlayerHighScore)));
		}
	
		if(CoinAmountTextBlock != nullptr)
		{
			CoinAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerData.PlayerCoins)));
		}
	}
}

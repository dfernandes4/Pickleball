// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeScreenWidget.h"

#include "CoinShopScreen.h"
#include "CollectionWidget.h"
#include "MainGamemode.h"
#include "MainPlayerController.h"
#include "PlayerPaddle.h"
#include "SaveGameInterface.h"
#include "SettingScreenWidget.h"
#include "ShopScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/AudioComponent.h"
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
	if(LeftArrowButton != nullptr)
	{
		LeftArrowButton->OnClicked.AddDynamic(this, &UHomeScreenWidget::OnLeftArrowClicked);
	}
	if(RightArrowButton != nullptr)
	{
		RightArrowButton->OnClicked.AddDynamic(this, &UHomeScreenWidget::OnRightArrowClicked);
	}
	if(LeaderboardButton != nullptr)
	{
		LeaderboardButton->OnClicked.AddDynamic(this, &UHomeScreenWidget::OnLeaderboardButtonClicked);
	}

	DisplayPlayerValues();
	
	AMainGamemode* MainGamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(MainGamemode)
	{
		MainGamemode->OnGameOver.AddDynamic(this, &UHomeScreenWidget::DisplayPlayerValues);
	}

	MainGamemode->OnCoinAmountChanged.AddDynamic(this, &UHomeScreenWidget::UpdateCoins);
	
	if(HomeScreenMusic)
	{
		HomeScreenMusicComponent = UGameplayStatics::SpawnSound2D(this,HomeScreenMusic);
	}
}

void UHomeScreenWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if(CollectionWidget != nullptr)
	{
		CollectionWidget->RemoveFromParent();
		CollectionWidget->OnCollectionClosed.RemoveDynamic(this, &UHomeScreenWidget::HandleChildClosed);
		CollectionWidget->OnPaddleSelected.RemoveDynamic(this, &UHomeScreenWidget::DisplayPaddles);
	}
	if(ShopWidget != nullptr)
	{
		ShopWidget->RemoveFromParent();
		ShopWidget->OnShopClosed.RemoveDynamic(this, &UHomeScreenWidget::HandleChildClosed);
	}
	if(HomeScreenMusicComponent)
	{
		HomeScreenMusicComponent->Stop();
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
	SettingScreenWidget->OnHowToPlayButtonClickedDelegate.AddDynamic(this, &UHomeScreenWidget::OnHowToPlayButtonClicked);
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UHomeScreenWidget::OnCollectionButtonClicked()
{
	if(CollectionWidget != nullptr)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
		CollectionWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UHomeScreenWidget::DisplayPaddles(UPaddleToCollectWidget* PaddleSelected, UPaddleToCollectWidget* PaddleBefore,
	UPaddleToCollectWidget* PaddleAfter)
{
	if(PaddleSelected != nullptr)
	{
		PaddleMiddle->SetVisibility(ESlateVisibility::Visible);
		
		TTuple<UObject*, const FVector2D&> PaddleSelectedImageInfo = PaddleSelected->GetPaddleImageInfo();
		PaddleMiddle->SetBrushFromTexture(Cast<UTexture2D>(PaddleSelectedImageInfo.Key));
		PaddleMiddle->SetDesiredSizeOverride(PaddleSelectedImageInfo.Value * 1.5);
	}
	else
	{
		PaddleMiddle->SetVisibility(ESlateVisibility::Hidden);
	}

	if(PaddleBefore != nullptr)
	{
		PaddleLeft->SetVisibility(ESlateVisibility::Visible);
		
		TTuple<UObject*, const FVector2D&> PaddleBeforeImageInfo = PaddleBefore->GetPaddleImageInfo();
		PaddleLeft->SetBrushFromTexture(Cast<UTexture2D>(PaddleBeforeImageInfo.Key));
		PaddleLeft->SetDesiredSizeOverride(PaddleBeforeImageInfo.Value * .75);

		LeftArrowButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PaddleLeft->SetVisibility(ESlateVisibility::Hidden);
		LeftArrowButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if(PaddleAfter != nullptr)
	{
		PaddleRight->SetVisibility(ESlateVisibility::Visible);
		RightArrowButton->SetVisibility(ESlateVisibility::Visible);
		
		TTuple<UObject*, const FVector2D&> PaddleAfterImageInfo = PaddleAfter->GetPaddleImageInfo();
		PaddleRight->SetBrushFromTexture(Cast<UTexture2D>(PaddleAfterImageInfo.Key));
		PaddleRight->SetDesiredSizeOverride(PaddleAfterImageInfo.Value * .75);
	}
	else
	{
		PaddleRight->SetVisibility(ESlateVisibility::Hidden);
		RightArrowButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHomeScreenWidget::OnShopButtonClicked()
{
	if(ShopWidget != nullptr)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);

		ShopWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	}
}

void UHomeScreenWidget::OnPlusCoinClicked()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	UCoinShopScreen* CoinShopScreenWidget = Cast<UCoinShopScreen>(WidgetLoader->LoadWidget(FName("CoinShopScreen"), GetWorld(),  1));
	CoinShopScreenWidget->OnCoinShopClosed.AddDynamic(this, &UHomeScreenWidget::HandleChildClosed);
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UHomeScreenWidget::OnLeftArrowClicked()
{
	CollectionWidget->SelectNewPaddle(CollectionWidget->CollectedPaddles[CollectionWidget->CollectedPaddles.Find(CollectionWidget->CurrentPaddleToCollectWidgetSelected) - 1]);
	APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerPaddle->OnPaddleSelected(*CollectionWidget->CurrentPaddleToCollectWidgetSelected->GetName());
}

void UHomeScreenWidget::OnRightArrowClicked()
{
	CollectionWidget->SelectNewPaddle(CollectionWidget->CollectedPaddles[CollectionWidget->CollectedPaddles.Find(CollectionWidget->CurrentPaddleToCollectWidgetSelected) + 1]);
	APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerPaddle->OnPaddleSelected(*CollectionWidget->CurrentPaddleToCollectWidgetSelected->GetName());
}

void UHomeScreenWidget::OnLeaderboardButtonClicked()
{
	//Change category tag accordingly
	//Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->ShowLeaderboard(FName("HS"));
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
        UE_LOG(LogTemp, Log, TEXT("Player High Score: %d"), PlayerData.PlayerHighScore);
        UE_LOG(LogTemp, Log, TEXT("Player Coins: %d"), PlayerData.PlayerCoins);

        if (HighScoreAmountTextBlock != nullptr)
        {
            HighScoreAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerData.PlayerHighScore)));
        }
    
        if (CoinAmountTextBlock != nullptr)
        {
            CoinAmountTextBlock->SetText(FText::FromString(FString::FromInt(PlayerData.PlayerCoins)));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SaveGameInterface is null"));
    }
}

void UHomeScreenWidget::DisplayBasePaddles()
{
	int32 CurrentPaddleSelectedIndex = CollectionWidget->CollectedPaddles.Find(CollectionWidget->CurrentPaddleToCollectWidgetSelected);

	if(CollectionWidget->CollectedPaddles.Num() == 1)
	{
		DisplayPaddles(CollectionWidget->CurrentPaddleToCollectWidgetSelected, nullptr, nullptr);
	}
	else
	{
		if(CurrentPaddleSelectedIndex == 0)
		{
			DisplayPaddles(CollectionWidget->CurrentPaddleToCollectWidgetSelected, nullptr, CollectionWidget->CollectedPaddles[CurrentPaddleSelectedIndex + 1]);
		}
		else if(CurrentPaddleSelectedIndex == CollectionWidget->CollectedPaddles.Num() - 1 || CollectionWidget->CollectedPaddles.Num() == 2)
		{
			DisplayPaddles(CollectionWidget->CurrentPaddleToCollectWidgetSelected, CollectionWidget->CollectedPaddles[CurrentPaddleSelectedIndex - 1], nullptr);
		}
		else
		{
			DisplayPaddles(CollectionWidget->CurrentPaddleToCollectWidgetSelected, CollectionWidget->CollectedPaddles[CurrentPaddleSelectedIndex - 1], CollectionWidget->CollectedPaddles[CurrentPaddleSelectedIndex + 1]);
		}
	}
}

void UHomeScreenWidget::SetUpCollectionDelegates()
{
	CollectionWidget->OnCollectionClosed.AddDynamic(this, &UHomeScreenWidget::HandleChildClosed);
	CollectionWidget->OnPaddleSelected.AddDynamic(this, &UHomeScreenWidget::DisplayPaddles);
}

void UHomeScreenWidget::SetUpShopDelegates()
{
	if(ShopWidget != nullptr)
	{
		ShopWidget->OnShopClosed.AddDynamic(this, &UHomeScreenWidget::HandleChildClosed);
	}
}

void UHomeScreenWidget::UpdateCoins(int32 NewCoinAmount)
{
	CoinAmountTextBlock->SetText(FText::FromString(FString::FromInt(NewCoinAmount)));
}

void UHomeScreenWidget::OnHowToPlayButtonClicked()
{
	RemoveFromParent();
}

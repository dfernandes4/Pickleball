// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopScreenWidget.h"

#include "CoinShopScreen.h"
#include "MainGamemode.h"
#include "PaddleToBuyWidget.h"
#include "PlayerPaddle.h"
#include "SaveGameInterface.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"



void UShopScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
	if(BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UShopScreenWidget::OnBackButtonClicked);
	}

	AMainGamemode* MainGamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGamemode->OnPaddleBought.AddDynamic(this, &UShopScreenWidget::SetupPaddleWidgets);

	SetupPaddleWidgets();
}

void UShopScreenWidget::OnBackButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
	OnShopClosed.Broadcast();
}

void UShopScreenWidget::SetupPaddleWidgets()
{
	ISaveGameInterface* SaveGameInterface = Cast<ISaveGameInterface>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (SaveGameInterface)
	{
		TMap<FName, bool> PaddleUnlockStatuses = SaveGameInterface->GetSaveGamePlayerData().PaddleUnlockStatuses;

		for(UWidget* Widget : CommonWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToBuyWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
				PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
			}
		}for(UWidget* Widget : RareWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToBuyWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
				PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
			}
		}
		for(UWidget* Widget : EpicWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToBuyWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
				PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
			}
		}
		for(UWidget* Widget : LegendaryWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToBuyWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
				PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
			}
		}
		for(UWidget* Widget : MythicWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToBuyWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
				PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
			}
		}
	}
	
	
}


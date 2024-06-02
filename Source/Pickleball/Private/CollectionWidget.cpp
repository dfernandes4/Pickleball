// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectionWidget.h"

#include "MainGamemode.h"
#include "PaddleToCollectWidget.h"
#include "PickleBallGameInstance.h"
#include "PlayerPaddle.h"
#include "SaveGameInterface.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"

void UCollectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
	// TODO: This is going to take some time to load so implement a loading screen here and do this this screen in beginning of game
	
	if(BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UCollectionWidget::OnBackButtonClicked);
	}
	
	AMainGamemode* MainGamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGamemode->OnPaddleSelected.AddDynamic(this, &UCollectionWidget::SelectNewPaddle);
	MainGamemode->OnPaddleBought.AddDynamic(this, &UCollectionWidget::SetupPaddleWidgets);

	ISaveGameInterface* SaveGameInterface = Cast<ISaveGameInterface>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (SaveGameInterface)
	{
		FPlayerData PlayerData = SaveGameInterface->GetSaveGamePlayerData();
		PaddleNameSelected = PlayerData.CurrentPaddleName;
	}
	
	SetupPaddleWidgets();
}

void UCollectionWidget::OnBackButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	OnCollectionClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

void UCollectionWidget::SetupPaddleWidgets()
{
	ISaveGameInterface* SaveGameInterface = Cast<ISaveGameInterface>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (SaveGameInterface)
	{
		CollectedPaddles.Empty();
		
		TMap<FName, bool> PaddleUnlockStatuses = SaveGameInterface->GetSaveGamePlayerData().PaddleUnlockStatuses;

		UPaddleToCollectWidget* NewSelectedPaddle = CurrentPaddleToCollectWidgetSelected;
		
		for(UWidget* Widget : CommonWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToCollectWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToCollectWidget* PaddleCollectWidget = Cast<UPaddleToCollectWidget>(Widget);
				PaddleCollectWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
				if(PaddleUnlockStatuses[*WidgetName])
				{
					CollectedPaddles.Add(PaddleCollectWidget);
					
					if(WidgetName == PaddleNameSelected)
					{
						NewSelectedPaddle = PaddleCollectWidget;
						APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
						PlayerPaddle->OnPaddleSelected(*WidgetName);
					}
				}
			}
		}
		for(UWidget* Widget : RareWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToCollectWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToCollectWidget* PaddleCollectWidget = Cast<UPaddleToCollectWidget>(Widget);
				PaddleCollectWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
				if(PaddleUnlockStatuses[*WidgetName])
				{
					CollectedPaddles.Add(PaddleCollectWidget);
					
					if(WidgetName == PaddleNameSelected)
					{
						NewSelectedPaddle = PaddleCollectWidget;
						APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
						PlayerPaddle->OnPaddleSelected(*WidgetName);
					}
				}
			}
		}
		for(UWidget* Widget : EpicWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToCollectWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToCollectWidget* PaddleCollectWidget = Cast<UPaddleToCollectWidget>(Widget);
				PaddleCollectWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
				if(PaddleUnlockStatuses[*WidgetName])
				{
					CollectedPaddles.Add(PaddleCollectWidget);

					if(WidgetName == PaddleNameSelected)
					{
						NewSelectedPaddle = PaddleCollectWidget;
						APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
						PlayerPaddle->OnPaddleSelected(*WidgetName);
					}
				}
			}
		}
		for(UWidget* Widget : LegendaryWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToCollectWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToCollectWidget* PaddleCollectWidget = Cast<UPaddleToCollectWidget>(Widget);
				PaddleCollectWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
				if(PaddleUnlockStatuses[*WidgetName])
				{
					CollectedPaddles.Add(PaddleCollectWidget);
					
					if(WidgetName == PaddleNameSelected)
					{
						NewSelectedPaddle = PaddleCollectWidget;
						APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
						PlayerPaddle->OnPaddleSelected(*WidgetName);
					}
				}
			}
		}
		for(UWidget* Widget : MythicWrapBox->GetAllChildren())
		{
			FString WidgetName = Cast<UPaddleToCollectWidget>(Widget)->GetName();
			if(PaddleUnlockStatuses.Contains(*WidgetName))
			{
				UPaddleToCollectWidget* PaddleCollectWidget = Cast<UPaddleToCollectWidget>(Widget);
				PaddleCollectWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
				if(PaddleUnlockStatuses[*WidgetName])
				{
					CollectedPaddles.Add(PaddleCollectWidget);
					
					if(WidgetName == PaddleNameSelected)
					{
						NewSelectedPaddle = PaddleCollectWidget;
						APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
						PlayerPaddle->OnPaddleSelected(*WidgetName);
					}
				}
			}
		}
		SelectNewPaddle(NewSelectedPaddle);
	}
}

void UCollectionWidget::SelectNewPaddle(UPaddleToCollectWidget* NewPaddleToCollectWidgetSelected)
{
	// Handle Check Image Transfer
	if(CurrentPaddleToCollectWidgetSelected != NewPaddleToCollectWidgetSelected)
	{
		if(CurrentPaddleToCollectWidgetSelected != nullptr)
		{
			CurrentPaddleToCollectWidgetSelected->CheckImage->SetVisibility(ESlateVisibility::Hidden);
		}
		NewPaddleToCollectWidgetSelected->CheckImage->SetVisibility(ESlateVisibility::Visible);

		CurrentPaddleToCollectWidgetSelected = NewPaddleToCollectWidgetSelected;
	}

	if(CurrentPaddleToCollectWidgetSelected != nullptr)
	{
		// Broadcast Quick select
		int32 CurrentPaddleSelectedIndex = CollectedPaddles.Find(CurrentPaddleToCollectWidgetSelected);

		if(CollectedPaddles.Num() == 1)
		{
			OnPaddleSelected.Broadcast(CurrentPaddleToCollectWidgetSelected, nullptr, nullptr);
		}
		else
		{
			if(CurrentPaddleSelectedIndex == 0)
			{
				OnPaddleSelected.Broadcast(CurrentPaddleToCollectWidgetSelected, nullptr, CollectedPaddles[CurrentPaddleSelectedIndex + 1]);
			}
			else if(CurrentPaddleSelectedIndex == CollectedPaddles.Num() - 1 || CollectedPaddles.Num() == 2)
			{
				OnPaddleSelected.Broadcast(CurrentPaddleToCollectWidgetSelected, CollectedPaddles[CurrentPaddleSelectedIndex - 1], nullptr);
			}
			else
			{
				OnPaddleSelected.Broadcast(CurrentPaddleToCollectWidgetSelected, CollectedPaddles[CurrentPaddleSelectedIndex - 1], CollectedPaddles[CurrentPaddleSelectedIndex + 1]);
			}
		}
	}
}

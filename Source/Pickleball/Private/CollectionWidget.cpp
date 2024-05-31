// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectionWidget.h"

#include "PaddleToCollectWidget.h"
#include "PlayerPaddle.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"

void UCollectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// TODO: This is going to take some time to load so implement a loading screen here and do this this screen in beginning of game
	
	if(BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UCollectionWidget::OnBackButtonClicked);
	}
	
	CurrentPaddleToCollectWidgetSelected = Cast<UPaddleToCollectWidget>(CommonWrapBox->GetChildAt(0));
	SetupPaddleWidgets();
}

void UCollectionWidget::OnBackButtonClicked()
{
	RemoveFromParent();
	OnCollectionClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

void UCollectionWidget::SetupPaddleWidgets()
{
	APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	TMap<FName, bool> PaddleUnlockStatuses = PlayerPaddle->GetPaddleUnlockStatuses();
	
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
			}
		}
	}
}

void UCollectionWidget::SelectNewPaddle(UPaddleToCollectWidget* NewPaddleToCollectWidgetSelected)
{
	CurrentPaddleToCollectWidgetSelected->CheckImage->SetVisibility(ESlateVisibility::Hidden);
	NewPaddleToCollectWidgetSelected->CheckImage->SetVisibility(ESlateVisibility::Visible);

	CurrentPaddleToCollectWidgetSelected = NewPaddleToCollectWidgetSelected;

	int32 CurrentPaddleSelectedIndex = CollectedPaddles.Find(CurrentPaddleToCollectWidgetSelected);
	int32 RandomIndex;
	do {
		RandomIndex = FMath::RandRange(0, CollectedPaddles.Num() - 1);
	} while (RandomIndex == CurrentPaddleSelectedIndex);
	
	if(CollectedPaddles.Num() >= 3)
	{
		int32 SecondRandomIndex;
		do {
			SecondRandomIndex = FMath::RandRange(0, CollectedPaddles.Num() - 1);
		} while (SecondRandomIndex == RandomIndex || SecondRandomIndex == CurrentPaddleSelectedIndex);
		OnPaddleSelected.Broadcast(CurrentPaddleToCollectWidgetSelected, CollectedPaddles[RandomIndex], CollectedPaddles[SecondRandomIndex]);
	}
	
	OnPaddleSelected.Broadcast(CurrentPaddleToCollectWidgetSelected, CollectedPaddles[RandomIndex], nullptr);
}

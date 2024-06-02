// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopScreenWidget.h"

#include "CoinShopScreen.h"
#include "PaddleToBuyWidget.h"
#include "PlayerPaddle.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"



void UShopScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UShopScreenWidget::OnBackButtonClicked);
	}
	
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
	APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	TMap<FName, bool> PaddleUnlockStatuses = PlayerPaddle->GetPaddleUnlockStatuses();
	
	for(UWidget* Widget : CommonWrapBox->GetAllChildren())
	{
		FString WidgetName = (Widget)->GetName();
		if(PaddleUnlockStatuses.Contains(*WidgetName))
		{
			UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
			PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
		}
	}for(UWidget* Widget : RareWrapBox->GetAllChildren())
	{
		FString WidgetName = (Widget)->GetName();
		if(PaddleUnlockStatuses.Contains(*WidgetName))
		{
			UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
			PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
		}
	}
	for(UWidget* Widget : EpicWrapBox->GetAllChildren())
	{
		FString WidgetName = (Widget)->GetName();
		if(PaddleUnlockStatuses.Contains(*WidgetName))
		{
			UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
			PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
		}
	}
	for(UWidget* Widget : LegendaryWrapBox->GetAllChildren())
	{
		FString WidgetName = (Widget)->GetName();
		if(PaddleUnlockStatuses.Contains(*WidgetName))
		{
			UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
			PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
		}
	}
	for(UWidget* Widget : MythicWrapBox->GetAllChildren())
	{
		FString WidgetName = (Widget)->GetName();
		if(PaddleUnlockStatuses.Contains(*WidgetName))
		{
			UPaddleToBuyWidget* PaddleToBuyWidget = Cast<UPaddleToBuyWidget>(Widget);
			PaddleToBuyWidget->SetPaddleAttributes(PaddleUnlockStatuses[*WidgetName]);
		}
	}
}


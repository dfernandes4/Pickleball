// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleToBuyScreenWidget.h"

#include "MainGamemode.h"
#include "PlayerPaddle.h"
#include "UserWidgetLoader.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPaddleToBuyScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UPaddleToBuyScreenWidget::OnBackButtonClicked);
	BuyButton->OnClicked.AddDynamic(this, &UPaddleToBuyScreenWidget::OnBuyButtonClicked);

	if(CloseScreenOverlayButton)
	{
		CloseScreenOverlayButton->OnClicked.AddDynamic(this, &UPaddleToBuyScreenWidget::OnBackButtonClicked);
		CloseScreenOverlayButton->SetIsEnabled(false);
	}
}

void UPaddleToBuyScreenWidget::OnBackButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	
	RemoveFromParent();
}

void UPaddleToBuyScreenWidget::OnBuyButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);

	APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(!PlayerPaddle->OnPaddleBought(PaddleToBuyImage->GetBrush().GetResourceName()))
	{
		UWidgetLoader* WidgetLoader = NewObject<UWidgetLoader>(this);
		WidgetLoader->LoadWidget(FName("CoinShopScreen"), GetWorld(), 5);
		// Play UI Error Sound and display a message that the player does not have enough coins
		RemoveFromParent();
	}
	else
	{
		// Play a sound effect for buying a paddle
		// Play Bounce effect of UI
		SetBackgroundHidden();
		UnlockedStarImage->SetVisibility(ESlateVisibility::Visible);
		
		PlayAnimation(PaddlePurchaseAnimation);
		const float PaddlePurchaseAnimationDuration = PaddlePurchaseAnimation->GetEndTime();
		
		FTimerHandle PaddlePurchaseTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(PaddlePurchaseTimerHandle, [this]()
		{
			CloseScreenOverlayButton->SetVisibility(ESlateVisibility::Visible);
			CloseScreenOverlayButton->SetIsEnabled(true);
			// Play Paddle float on loop
			PlayAnimation(PaddleFloatingAnimation, 0, 0.0f, EUMGSequencePlayMode::Forward, 1.0f, true);
			
		}, PaddlePurchaseAnimationDuration, false);
		
		AMainGamemode* MainGamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
		MainGamemode->OnPaddleBought.Broadcast();
	}
}

void UPaddleToBuyScreenWidget::SetPaddleAttributes(UObject* ResourceObject, const FVector2D& ImageSize, const FString& PaddleName, const EPaddleRarity& PaddleRarity)
{
	PaddleToBuyImage->SetBrushFromTexture(Cast<UTexture2D>(ResourceObject));
	PaddleToBuyImage->SetDesiredSizeOverride(ImageSize * 2.4);

	PaddleNameTextBlock->SetText(FText::FromString(PaddleName));
	UnlockedPaddleNameTextBlock->SetText(FText::FromString(PaddleName));
	switch (PaddleRarity)
	{
		case EPaddleRarity::Common:
			UnlockedPaddleNameTextBlock->SetColorAndOpacity(FLinearColor::White);
			break;
		case EPaddleRarity::Rare:
			UnlockedPaddleNameTextBlock->SetColorAndOpacity(FLinearColor(0.177083, 0.794271, 1.0, 1.0));
			break;
		case EPaddleRarity::Epic:
			UnlockedPaddleNameTextBlock->SetColorAndOpacity(FLinearColor(0.553192, 0.166081, 1.0, 1.0));
			break;
		case EPaddleRarity::Legendary:
			UnlockedPaddleNameTextBlock->SetColorAndOpacity(FLinearColor(1, .8, 0, 1.0));
			break;
		case EPaddleRarity::Mythic:
			UnlockedPaddleNameTextBlock->SetColorAndOpacity(FLinearColor(1, 0, 0.0235, 1.0));
			break;
		default:
			break;
	}
}

void UPaddleToBuyScreenWidget::SetBackgroundHidden()
{
	BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
	BackButton->SetVisibility(ESlateVisibility::Hidden);
	BuyButton->SetVisibility(ESlateVisibility::Hidden);
	PaddleNameTextBlock->SetVisibility(ESlateVisibility::Hidden);
}



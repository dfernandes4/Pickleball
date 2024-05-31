// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleToBuyScreenWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPaddleToBuyScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UPaddleToBuyScreenWidget::OnBackButtonClicked);
	BuyButton->OnClicked.AddDynamic(this, &UPaddleToBuyScreenWidget::OnBuyButtonClicked);
}

void UPaddleToBuyScreenWidget::OnBackButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	
	RemoveFromParent();
}

void UPaddleToBuyScreenWidget::OnBuyButtonClicked()
{
	// Play Bounce effect of UI
	// Maybe play a different noise for buying a paddle
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);

	/*
	 * The paddle is added to the player's inventory and the player's coin count is decremented by the cost of the paddle
	 * Do this by check if PaddleUnlockStatuses.Contains(PaddleName), if so then PaddleUnlockStatuses[PaddleName] = true
	 * Then take the number in the PaddleCoinAmount and subtract it from the player's coin count
	 * 
	 * The player's inventory is saved to the player's save file
	 * The player's coin count is saved to the player's save file
	 */
	
	RemoveFromParent();
}

void UPaddleToBuyScreenWidget::SetPaddleAttributes(UObject* ResourceObject, const FVector2D& ImageSize, const FString& PaddleName)
{
	PaddleToBuyImage->SetBrushFromTexture(Cast<UTexture2D>(ResourceObject));
	PaddleToBuyImage->SetDesiredSizeOverride(ImageSize * 2.4);

	PaddleNameTextBlock->SetText(FText::FromString(PaddleName));
}



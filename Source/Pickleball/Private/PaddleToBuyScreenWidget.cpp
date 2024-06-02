// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleToBuyScreenWidget.h"

#include "MainGamemode.h"
#include "PlayerPaddle.h"
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

	APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(!PlayerPaddle->OnPaddleBought(PaddleToBuyImage->GetBrush().GetResourceName()))
	{
		// Play UI Error Sound and display a message that the player does not have enough coins
	}
	else
	{
		AMainGamemode* MainGamemode = Cast<AMainGamemode>(UGameplayStatics::GetGameMode(GetWorld()));
		MainGamemode->OnPaddleBought.Broadcast();
	}
	
	// TODO: Either the paddle to buy icon is removed from wrap box or its opacity is set to 0.5 and coin amount is hidden and button cant be pressed
	// Play a sound effect for buying a paddle
	
	RemoveFromParent();
}

void UPaddleToBuyScreenWidget::SetPaddleAttributes(UObject* ResourceObject, const FVector2D& ImageSize, const FString& PaddleName)
{
	PaddleToBuyImage->SetBrushFromTexture(Cast<UTexture2D>(ResourceObject));
	PaddleToBuyImage->SetDesiredSizeOverride(ImageSize * 2.4);

	PaddleNameTextBlock->SetText(FText::FromString(PaddleName));
}



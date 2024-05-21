// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinShopScreen.h"

#include "MainPlayerController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCoinShopScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if(BackButton != nullptr)
	{
		BackButton->OnPressed.AddDynamic(this, &UCoinShopScreen::OnBackButtonPressed);
	}

	if(SomeGoldButton != nullptr)
	{
		SomeGoldButton->OnPressed.AddDynamic(this, &UCoinShopScreen::OnSomeGoldButtonPressed);

	}
	if(FistOGoldButton != nullptr)
	{
		FistOGoldButton->OnPressed.AddDynamic(this, &UCoinShopScreen::OnFistOGoldButtonPressed);

	}

	if(LotsOGoldButton != nullptr)
	{
		LotsOGoldButton->OnPressed.AddDynamic(this, &UCoinShopScreen::OnLotsOGoldButtonPressed);
	}

	if(PilesOGoldButton != nullptr)
	{
		PilesOGoldButton->OnPressed.AddDynamic(this, &UCoinShopScreen::OnPilesOGoldButtonPressed);
	}
}

void UCoinShopScreen::OnBackButtonPressed()
{
	RemoveFromParent();
	OnCoinShopClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

void UCoinShopScreen::OnSomeGoldButtonPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("");
}

void UCoinShopScreen::OnFistOGoldButtonPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("");
}

void UCoinShopScreen::OnLotsOGoldButtonPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("");
}

void UCoinShopScreen::OnPilesOGoldButtonPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("");
}



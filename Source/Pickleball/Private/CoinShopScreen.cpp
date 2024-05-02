// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinShopScreen.h"

#include "MainPlayerController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCoinShopScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if(ExitBtn != nullptr)
	{
		ExitBtn->OnPressed.AddDynamic(this, &UCoinShopScreen::OnExitButtonPressed);
	}

	if(SomeGoldBtn != nullptr)
	{
		SomeGoldBtn->OnPressed.AddDynamic(this, &UCoinShopScreen::OnSomeGoldBtnPressed);

	}

	if(LotsofGoldBtn != nullptr)
	{
		LotsofGoldBtn->OnPressed.AddDynamic(this, &UCoinShopScreen::OnLotsofGoldBtnPressed);
	}

	if(TonsofGoldBtn != nullptr)
	{
		TonsofGoldBtn->OnPressed.AddDynamic(this, &UCoinShopScreen::OnTonsofBtnPressed);
	}
}

void UCoinShopScreen::OnExitButtonPressed()
{
	RemoveFromParent();
	OnCoinShopClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

void UCoinShopScreen::OnSomeGoldBtnPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("");
}

void UCoinShopScreen::OnLotsofGoldBtnPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("");
}

void UCoinShopScreen::OnTonsofBtnPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("");
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinShopScreen.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCoinShopScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if(ExitBtn != nullptr)
	{
		ExitBtn->OnClicked.AddDynamic(this, &UCoinShopScreen::OnExitButtonPressed);
	}
}

void UCoinShopScreen::OnExitButtonPressed()
{
	RemoveFromParent();
	OnCoinShopClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

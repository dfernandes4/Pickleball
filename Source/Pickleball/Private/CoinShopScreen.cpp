// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinShopScreen.h"

#include "MainPlayerController.h"
#include "PickleBallGameInstance.h"
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
	AMainPlayerController* MainPlayerController =Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	if(MainPlayerController!= nullptr)
	{
		MainPlayerController->OnPurchaseCompleted.AddDynamic(this, &UCoinShopScreen::OnPurchaseCompleted);
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
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("Some_Gold_");
}

void UCoinShopScreen::OnFistOGoldButtonPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("Fist_O_Gold_");
}

void UCoinShopScreen::OnLotsOGoldButtonPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("Lots_O_Gold_");
}

void UCoinShopScreen::OnPilesOGoldButtonPressed()
{
	Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController())->InitiatePurchaseRequest("Piles_O_Gold_");
}


void UCoinShopScreen::OnPurchaseCompleted(int32 CoinAmount)
{
	/*
	int32 CoinsAmount = 0;

	if(ProductId == "Some_Gold_")
	{
		CoinsAmount = 200;
	}
	else if(ProductId == "Fist_O_Gold_")
	{
		CoinsAmount = 400;
	}
	else if(ProductId == "Lots_O_Gold_")
	{
		CoinsAmount = 650;
	}
	else if(ProductId == "Piles_O_Gold_")
	{
		CoinsAmount = 1500;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Product Id is not valid."));
		return;
	}
	OnPurchaseCompletedDelegate.Broadcast(CoinsAmount);
	*/
	UGameplayStatics::PlaySound2D(GetWorld(),CoinPurchasedSound);
	
}

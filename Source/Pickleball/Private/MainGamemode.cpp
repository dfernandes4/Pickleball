// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGamemode.h"

#include "CollectionWidget.h"
#include "HomeScreenWidget.h"
#include "PickleBallGameInstance.h"
#include "ShopScreenWidget.h"
#include "Sound/SoundClass.h"
#include "UserWidgetLoader.h"

AMainGamemode::AMainGamemode()
{
	if(MasterSoundClass != nullptr)
	{
		MasterSoundClass->Properties.Volume = .8;
	}
	if(SFXSoundClass != nullptr)
	{
		SFXSoundClass->Properties.Volume = .8;
	}
	if(MusicSoundClass != nullptr)
	{
		MusicSoundClass->Properties.Volume = .8;
	}
	OnGameOver.AddDynamic(this, &AMainGamemode::GameOver);
}

void AMainGamemode::BeginPlay()
{
	Super::BeginPlay();
	
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetWorld()->GetGameInstance());
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if(GameInstance->GetShouldLaunchStarterScreen())
	{
		if(GameInstance->GetIsFirstTimePlaying())
		{
			WidgetLoader->LoadWidget(FName("TutorialScreen"), GetWorld());
			GameInstance->SetShouldLaunchStarterScreen(false);
		}
		else
		{
			UHomeScreenWidget* HomeScreenWidget = Cast<UHomeScreenWidget>(WidgetLoader->LoadWidget(FName("HomeScreen"), GetWorld()));
			HomeScreenWidget->CollectionWidget	= Cast<UCollectionWidget>(WidgetLoader->LoadWidget(FName("CollectionScreen"), GetWorld()));
			HomeScreenWidget->DisplayBasePaddles();
			HomeScreenWidget->SetUpCollectionDelegates();
			HomeScreenWidget->ShopWidget		= Cast<UShopScreenWidget>(WidgetLoader->LoadWidget(FName("PaddleShopScreen"), GetWorld()));
			HomeScreenWidget->SetUpShopDelegates();
			GameInstance->SetShouldLaunchStarterScreen(false);
		}

		PlayerController->DisableInput(PlayerController);
	}
	else
	{
		WidgetLoader->LoadWidget(FName("Countdown"), GetWorld());
		PlayerController->EnableInput(PlayerController);
	}
}

void AMainGamemode::GameOver()
{
	UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetWorld()->GetGameInstance());
	if(GameInstance->GetIsFirstTimePlaying())
	{
		GameInstance->SetIsFirstTimePlaying(false);
	}
	OnGameOver.RemoveDynamic(this, &AMainGamemode::GameOver);
}

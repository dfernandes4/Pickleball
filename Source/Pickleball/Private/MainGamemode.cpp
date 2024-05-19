// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGamemode.h"

#include "PickleBallGameInstance.h"
#include "Sound/SoundClass.h"
#include "UserWidgetLoader.h"
#include "Kismet/GameplayStatics.h"


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
	
}

void AMainGamemode::BeginPlay()
{
	Super::BeginPlay();
	
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	UPickleBallGameInstance* GameInstance = Cast<UPickleBallGameInstance>(GetWorld()->GetGameInstance());
	
	if(GameInstance->GetIsFirstTimePlaying())
	{
		WidgetLoader->LoadWidget(FName("TutorialScreen"), GetWorld());
	}
	else
	{
		WidgetLoader->LoadWidget(FName("HomeScreen"), GetWorld());
	}
}

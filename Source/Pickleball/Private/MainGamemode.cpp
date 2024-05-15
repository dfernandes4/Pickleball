// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGamemode.h"

#include "PlayerPaddle.h"
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
}

void AMainGamemode::BeginPlay()
{
	Super::BeginPlay();

	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("HomeScreen"), GetWorld());
}

void AMainGamemode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if(PlayerPaddle != nullptr)
	{
		PlayerPaddle->SaveAllStats();
	}
}

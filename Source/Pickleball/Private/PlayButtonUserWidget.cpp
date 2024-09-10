// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayButtonUserWidget.h"

#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPlayButtonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	if(PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UPlayButtonUserWidget::PlayButtonClicked);
	}
}

void UPlayButtonUserWidget::PlayButtonClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("Countdown"), GetWorld());
}

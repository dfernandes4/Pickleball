// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayButtonUserWidget.h"

#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPlayButtonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UPlayButtonUserWidget::PlayButtonClicked);
	}
}

void UPlayButtonUserWidget::PlayButtonClicked()
{
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("Countdown"), GetWorld());

	RemoveFromParent();
}

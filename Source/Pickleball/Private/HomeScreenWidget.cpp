// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeScreenWidget.h"

#include "UserWidgetLoader.h"
#include "Components/Button.h"


void UHomeScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(PlayButton!= nullptr)
	{
		PlayButton->OnClicked.AddDynamic(this,&UHomeScreenWidget::OnPlayButtonClicked);
	}
}

void UHomeScreenWidget::OnPlayButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("Countdown"), GetWorld());
	
}


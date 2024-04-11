// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeScreenWidget.h"

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
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverScreenWidget.h"

#include "HomeScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UGameOverScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ReplayButton!= nullptr)
	{
		ReplayButton->OnClicked.AddDynamic(this,&UGameOverScreenWidget::OnReplayButtonClicked);
	}

	if(HomeButton!= nullptr)
	{
		HomeButton->OnClicked.AddDynamic(this,&UGameOverScreenWidget::OnHomeButtonClicked);
	}

	if(WatchAdd2xCoinsButton!= nullptr)
	{
		WatchAdd2xCoinsButton->OnClicked.AddDynamic(this,&UGameOverScreenWidget::OnWatchAdd2xCoinsButtonClicked);
	}

	if(WatchAddContinueButton!= nullptr)
	{
		WatchAddContinueButton->OnClicked.AddDynamic(this,&UGameOverScreenWidget::OnWatchAddContinueButtonClicked);
	}
	

	
}

void UGameOverScreenWidget::OnReplayButtonClicked()
{
	
}

void UGameOverScreenWidget::OnHomeButtonClicked()
{
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("CollectionScreen"), GetWorld(),  1);
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);

	RemoveFromParent();
}

void UGameOverScreenWidget::OnWatchAdd2xCoinsButtonClicked()
{
	
}

void UGameOverScreenWidget::OnWatchAddContinueButtonClicked()
{
	
}

void UGameOverScreenWidget::HandleChildClosed()
{
	SetVisibility(ESlateVisibility::Visible);
}




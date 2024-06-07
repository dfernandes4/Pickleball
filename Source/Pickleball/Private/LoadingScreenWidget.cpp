// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenWidget.h"

void ULoadingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadingScreenDuration = 1.25;

	if(LoadingAnimation != nullptr)
	{
		PlayAnimation(LoadingAnimation);
		FTimerHandle LoadingScreenTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(LoadingScreenTimerHandle, [this]()
		{
			RemoveFromParent();
		}, LoadingScreenDuration, false);
	}
}

void ULoadingScreenWidget::NativeDestruct()
{
	Super::NativeDestruct();
	LoadingScreenFinished.Broadcast();
}

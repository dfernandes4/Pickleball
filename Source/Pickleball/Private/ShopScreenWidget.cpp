// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopScreenWidget.h"

#include "CoinShopScreen.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"



void UShopScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UShopScreenWidget::OnBackButtonClicked);
	}
}

void UShopScreenWidget::OnBackButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
	RemoveFromParent();
	OnShopClosed.Broadcast();
}


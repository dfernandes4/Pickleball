// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopScreenWidget.h"

#include "Components/Button.h"

void UShopScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(ExitBtn != nullptr)
	{
		ExitBtn->OnClicked.AddDynamic(this, &UShopScreenWidget::OnExitButtonClicked);
	}
}

void UShopScreenWidget::OnExitButtonClicked()
{
	RemoveFromParent();
	OnShopClosed.Broadcast();
}

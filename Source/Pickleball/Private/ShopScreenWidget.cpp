// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopScreenWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

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

	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

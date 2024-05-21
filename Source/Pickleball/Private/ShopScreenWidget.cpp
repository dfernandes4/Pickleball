// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopScreenWidget.h"

#include "CoinShopScreen.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"



void UShopScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UShopScreenWidget::OnBackButtonClicked);
	}

	if(AddCoinsBtn != nullptr)
	{
		AddCoinsBtn->OnClicked.AddDynamic(this,&UShopScreenWidget::OnAddCoinsBtnClicked);
	}
}

void UShopScreenWidget::OnBackButtonClicked()
{
	RemoveFromParent();
	OnShopClosed.Broadcast();

	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

void UShopScreenWidget::OnAddCoinsBtnClicked()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	UCoinShopScreen* CoinShopScreenWidget = Cast<UCoinShopScreen>(WidgetLoader->LoadWidget(FName("CoinShopScreen"), GetWorld(),  1));
	CoinShopScreenWidget->OnCoinShopClosed.AddDynamic(this, &UShopScreenWidget::HandleChildClosed);
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}



void UShopScreenWidget::HandleChildClosed()
{
		SetVisibility(ESlateVisibility::Visible);
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleToBuyWidget.h"

#include "PaddleToBuyScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPaddleToBuyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PaddleToBuyBtn->OnClicked.AddDynamic(this, &UPaddleToBuyWidget::OnPaddleButtonClicked);
}

void UPaddleToBuyWidget::OnPaddleButtonClicked()
{
	FButtonStyle ButtonStyle = PaddleToBuyBtn->GetStyle();
	FVector2D ImageSize = ButtonStyle.Normal.GetImageSize();
	UObject* ResourceObject = ButtonStyle.Normal.GetResourceObject();

	// Get the name of the paddle that is being displayed
	FName TextureName = ButtonStyle.Normal.GetResourceName();
	TArray<FString> TextureNameArray;
	TextureName.ToString().ParseIntoArray(TextureNameArray, TEXT("_"));
	FString PaddleName;
	for(FString Word : TextureNameArray)
	{
		PaddleName += Word;
		if(Word != TextureNameArray[TextureNameArray.Num() - 1])
		{
			PaddleName += " ";
		}
	}
	
	UWidgetLoader* WidgetLoader = NewObject<UWidgetLoader>(this);
	UPaddleToBuyScreenWidget* PaddleToBuyScreenWidget = Cast<UPaddleToBuyScreenWidget>(WidgetLoader->LoadWidget(FName("PaddleToBuyScreen"), GetWorld(), 5));
	PaddleToBuyScreenWidget->SetPaddleAttributes(ResourceObject, ImageSize, PaddleName, PaddleRarity);
	
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}

void UPaddleToBuyWidget::SetPaddleAttributes(bool bIsUnlocked)
{
	if(bIsUnlocked)
	{
		PaddleToBuyBtn->SetIsEnabled(false);
		CoinImage->SetVisibility(ESlateVisibility::Collapsed);
		PaddleAmountTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

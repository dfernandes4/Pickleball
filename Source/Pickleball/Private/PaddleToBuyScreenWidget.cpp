// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleToBuyScreenWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPaddleToBuyScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UPaddleToBuyScreenWidget::OnBackButtonClicked);
}

void UPaddleToBuyScreenWidget::OnBackButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	
	RemoveFromParent();
}

void UPaddleToBuyScreenWidget::SetPaddleAttributes(UObject* ResourceObject, const FVector2D& ImageSize, const FString& PaddleName)
{
	PaddleToBuyImage->SetBrushFromTexture(Cast<UTexture2D>(ResourceObject));
	PaddleToBuyImage->SetDesiredSizeOverride(ImageSize * 2.4);

	PaddleNameTextBlock->SetText(FText::FromString(PaddleName));
}

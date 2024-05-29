// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleToSelectScreenWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPaddleToSelectScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UPaddleToSelectScreenWidget::OnBackButtonClicked);
}

void UPaddleToSelectScreenWidget::OnBackButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	
	RemoveFromParent();
}

void UPaddleToSelectScreenWidget::SetPaddleAttributes(UObject* ResourceObject, const FVector2D& ImageSize, const FString& PaddleName)
{
	PaddleToSelectImage->SetBrushFromTexture(Cast<UTexture2D>(ResourceObject));
	PaddleToSelectImage->SetDesiredSizeOverride(ImageSize * 2);

	PaddleNameTextBlock->SetText(FText::FromString(PaddleName));
}

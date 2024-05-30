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
	UseButton->OnClicked.AddDynamic(this, &UPaddleToSelectScreenWidget::OnUseButtonClicked);
}

void UPaddleToSelectScreenWidget::OnBackButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	
	RemoveFromParent();
}

void UPaddleToSelectScreenWidget::OnUseButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
	
	// The paddle that was previously selected is deselected (hiding the green check) and the new paddle is selected (showing the green check)
	// Player's Paddle Sprite, Paddle Sound effect, and Paddle Ability (if applicable) are updated
	// Home screen's paddle selection is updated to reflect the player's new paddle, and paddle position updates on screen as well
	// The player's selected paddle is saved to the player's save file

	RemoveFromParent();
}

void UPaddleToSelectScreenWidget::SetPaddleAttributes(UObject* ResourceObject, const FVector2D& ImageSize, const FString& PaddleName)
{
	PaddleToSelectImage->SetBrushFromTexture(Cast<UTexture2D>(ResourceObject));
	PaddleToSelectImage->SetDesiredSizeOverride(ImageSize * 2);

	PaddleNameTextBlock->SetText(FText::FromString(PaddleName));
}

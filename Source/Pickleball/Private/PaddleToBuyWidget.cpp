// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleToBuyWidget.h"

#include "PaddleToBuyScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
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
	PaddleToBuyScreenWidget->SetPaddleAttributes(ResourceObject, ImageSize, PaddleName);
	
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);

	// User clicks confirm once that happens the paddle that is displayed is either selected or is "bought" and added to the player's inventory
	// If the paddle is selected, the paddle that was previously selected is deselected and the new paddle is selected
	// If the paddle is "bought" the paddle is added to the player's inventory and the player's coin count is decremented by the cost of the paddle
	
	// The player's inventory is saved to the player's save file
	// The player's coin count is saved to the player's save file
	// The player's selected paddle is saved to the player's save file
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddleToCollectWidget.h"

#include "CollectionWidget.h"
#include "PaddleToSelectScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPaddleToCollectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PaddleToCollectBtn->OnClicked.AddDynamic(this, &UPaddleToCollectWidget::OnPaddleButtonClicked);
}

void UPaddleToCollectWidget::OnPaddleButtonClicked()
{
	FButtonStyle ButtonStyle = PaddleToCollectBtn->GetStyle();
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
	UPaddleToSelectScreenWidget* PaddleToSelectScreenWidget = Cast<UPaddleToSelectScreenWidget>(WidgetLoader->LoadWidget(FName("PaddleToSelectScreen"), GetWorld(), 5));
	PaddleToSelectScreenWidget->SetPaddleAttributes(ResourceObject, ImageSize, PaddleName);
	
	UGameplayStatics::PlaySound2D(GetWorld(), MenuSoundEffect);
}


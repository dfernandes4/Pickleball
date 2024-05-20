// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectionWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCollectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UCollectionWidget::OnBackButtonClicked);
	}
}

void UCollectionWidget::OnBackButtonClicked()
{
	RemoveFromParent();
	OnCollectionClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectionWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCollectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &UCollectionWidget::OnExitButtonClicked);
	}
}

void UCollectionWidget::OnExitButtonClicked()
{
	RemoveFromParent();
	OnCollectionClosed.Broadcast();
	UGameplayStatics::PlaySound2D(GetWorld(), BackSoundEffect);
}

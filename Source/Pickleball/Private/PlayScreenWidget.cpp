// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayScreenWidget.h"

#include "MainGamemode.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

 

void UPlayScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());

	if(MainGamemode)
	{
		MainGamemode->OnScoreUpdated.AddDynamic(this, &UPlayScreenWidget::UpdateScore);
	}
}

void UPlayScreenWidget::UpdateScore(int NewScore)
{
	ScoreText->SetText(FText::FromString(FString::FromInt(NewScore)));

	UGameplayStatics::PlaySound2D(GetWorld(), PointSoundEffect);
}
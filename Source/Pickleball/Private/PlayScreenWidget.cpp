// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayScreenWidget.h"

#include "MainGamemode.h"
#include "SettingScreenWidget.h"
#include "UserWidgetLoader.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

class UWidgetLoader;

void UPlayScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainGamemode = Cast<AMainGamemode>(GetWorld()->GetAuthGameMode());

	if(MainGamemode)
	{
		MainGamemode->OnScoreUpdated.AddDynamic(this, &UPlayScreenWidget::UpdateScore);
	}

	if(PauseButton)
	{
		PauseButton->OnClicked.AddDynamic(this, &UPlayScreenWidget::PauseButtonClicked);
	}
}

void UPlayScreenWidget::UpdateScore(int NewScore)
{
	ScoreText->SetText(FText::FromString(FString::FromInt(NewScore)));
}

void UPlayScreenWidget::PauseButtonClicked()
{
	bool bIsCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
	UGameplayStatics::SetGamePaused(GetWorld(), !bIsCurrentlyPaused);

	SetVisibility(ESlateVisibility::HitTestInvisible);
	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	USettingScreenWidget* SettingScreenWidget = Cast<USettingScreenWidget>(WidgetLoader->LoadWidget(FName("SettingScreen"), GetWorld(),  1));
	SettingScreenWidget->OnSettingsClosed.AddDynamic(this, &UPlayScreenWidget::HandlePauseClosed);
}

void UPlayScreenWidget::HandlePauseClosed()
{
	SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
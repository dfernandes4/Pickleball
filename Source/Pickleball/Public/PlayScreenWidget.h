// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UPlayScreenWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PlayingHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PlayingHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> PauseButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PlayingHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UImage> EnemyIconImage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PlayingHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> EnemyNameText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PlayingHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ScoreText;
	
};

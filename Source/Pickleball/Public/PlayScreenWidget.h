// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayScreenWidget.generated.h"

class AMainGamemode;
/**
 * 
 */
UCLASS()
class PICKLEBALL_API UPlayScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateScore(int NewScore);

	UFUNCTION()
	void HandlePauseClosed();
	UFUNCTION()
	void PauseButtonClicked();
	UFUNCTION()
	void HandleGameOver();
	UFUNCTION()
	void SetRandomEnemyAttributes();
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* PointSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* PauseSoundEffect;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PLayerStats, meta = (AllowPrivateAccess))
	AMainGamemode* MainGamemode;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HUDSlideInAnimation;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopScreenWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopClosedDelegate);

UCLASS()
class PICKLEBALL_API UShopScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnExitButtonClicked();
	
	UPROPERTY()
	FOnShopClosedDelegate OnShopClosed;

private:

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> ExitBtn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> CoinIconAddBtn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> AddCoinsBtn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton>	TotalCoinAmountBtn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBackgroundBlur> BackgroundBlur;
	
};

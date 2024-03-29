// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UShopScreenWidget : public UUserWidget
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> ExitButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Tier1CrateButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShopHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Tier2CrateButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShoPHud", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Tier3CrateButton;
	

	
	
};

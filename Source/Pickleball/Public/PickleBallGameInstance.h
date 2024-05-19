// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGameInterface.h"
#include "Engine/GameInstance.h"
#include "PickleBallGameInstance.generated.h"


class UHomeScreenWidget;
/**
 * 
 */
UCLASS()
class PICKLEBALL_API UPickleBallGameInstance : public UGameInstance, public ISaveGameInterface
{
	GENERATED_BODY()

public:

	UPickleBallGameInstance();
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION()
	virtual void LoadGameData() override;
	UFUNCTION()
	virtual void SaveGameData() override;
	UFUNCTION()
	virtual FPlayerData GetSaveGamePlayerData() override;
	UFUNCTION()
	virtual void SavePlayerData(FPlayerData PlayerData) override;

	UFUNCTION()
	bool GetIsFirstTimePlaying();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	UPickleballSaveGame* SaveGame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	FString SlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	bool bIsFirstTimePlaying;
};

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
	bool GetIsFirstTimePlaying() const;
	UFUNCTION()
	bool GetShouldLaunchStarterScreen() const;
	UFUNCTION()
	void SetShouldLaunchStarterScreen(bool bIShouldLaunchStarterScreenIn);

	UFUNCTION()
	void SetIsFirstTimePlaying(bool bIsFirstTimePlayingIn);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	UPickleballSaveGame* SaveGame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	FString SlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	bool bIsFirstTimePlaying;
	
	bool bShouldLaunchStarterScreen = true;

	bool bIsFirstHomeScreenLaunch = true;
};

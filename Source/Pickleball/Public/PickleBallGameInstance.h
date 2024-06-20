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

	virtual void LoadGameData() override;
	virtual void SaveGameData() override;
	virtual FPlayerData GetSaveGamePlayerData() override;
	virtual void SavePlayerData(FPlayerData PlayerData) override;
	virtual void SaveCurrentEnemyRow(int32 EnemyRow) override;
	
	UFUNCTION()
	bool GetIsFirstTimePlaying() const;
	UFUNCTION()
	bool GetShouldLaunchStarterScreen() const;
	UFUNCTION()
	void SetShouldLaunchStarterScreen(bool bIShouldLaunchStarterScreenIn);

	UFUNCTION()
	void SetIsFirstTimePlaying(bool bIsFirstTimePlayingIn);
	UFUNCTION()
	int32 GetSaveGameEnemyRow();

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

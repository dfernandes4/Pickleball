// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGameInterface.h"
#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "PickleBallGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadFinishedDelegate);

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

	bool OnCloudLoadCompleted(const FString& FileName);
	virtual void LoadGameData() override;
	virtual void SaveGameData() override;
	virtual FPlayerData GetSaveGamePlayerData() override;
	virtual void SavePlayerData(FPlayerData PlayerData) override;
	virtual void SaveCurrentEnemyRow(int32 EnemyRow) override;
	
	UFUNCTION()
	bool GetIsFirstTimePlaying() const;
	UFUNCTION()
	bool GetShouldLaunchStarterScreen() const;
	void OnLoadFinished(const FString& SlotNameIn, int32 UserIndex, USaveGame* LoadedGame);
	void RetryLoad();
	UFUNCTION()
	void SetShouldLaunchStarterScreen(bool bIShouldLaunchStarterScreenIn);

	UFUNCTION()
	void SetIsFirstTimePlayingEver(bool bIsFirstTimePlayingEverIn);
	UFUNCTION()
	bool GetIsFirstTimePlayingInSession() const;
    UFUNCTION()
	void SetIsFirstTimePlayingInSession(bool bIsFirstTimePlayingInSessionIn);
    UFUNCTION()
    bool GetIsGameLoaded() const;
    
	UFUNCTION()
	int32 GetSaveGameEnemyRow();
	

	UPROPERTY()
	FLoadFinishedDelegate LoadFinished;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	UPickleballSaveGame* SaveGame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	FString SlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	bool bIsFirstTimePlayingEver;

	UPROPERTY()
	bool bIsFirstTimePlayingInSession;
	
	bool bShouldLaunchStarterScreen;

	bool bIsFirstHomeScreenLaunch;
    
    bool bIsGameLoaded;

	int32 RetryCount;
	int32 MaxRetries;
	float RetryDelay;
};

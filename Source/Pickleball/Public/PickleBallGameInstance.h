// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGameInterface.h"
#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "PickleBallGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadFinishedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAdsRemovedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAdButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRewardFinished);

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
    UFUNCTION(BlueprintCallable)
    int32 GetCurrentGameCount() const;
    UFUNCTION(BlueprintCallable)
    void IncrementCurrentGameCount();
	
	UFUNCTION(BlueprintCallable)
	bool GetIsFirstTimePlaying() const;
	UFUNCTION(BlueprintCallable)
	bool GetShouldLaunchStarterScreen() const;
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
    UFUNCTION(BlueprintCallable)
    void RewardFinishedComplete() const;
    
	UFUNCTION()
	int32 GetSaveGameEnemyRow();
    
    UFUNCTION(BlueprintCallable)
    void RemoveAds();
    
    UFUNCTION(BlueprintCallable)
    bool AreAdsEnabled() const;
	
	UPROPERTY()
	FLoadFinishedDelegate LoadFinished;
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAdsRemovedDelegate AdsRemoved;
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAdButtonPressed OnAdButtonPressed;
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRewardFinished RewardFinished;
	
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
    
    int32 CurrentGameCount;
};

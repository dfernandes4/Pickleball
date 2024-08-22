// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGameInterface.h"
#include "SettingScreenWidget.h"
#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "Interfaces/OnlineIdentityInterface.h"
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

	// Constructor / Destructor
	UPickleBallGameInstance();
	virtual void Init() override;
	virtual void Shutdown() override;

	// Loading
	virtual void LoadGameData() override;
	UFUNCTION()
	void LoginToGameCenter();
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	bool SetupValidSaveGame(const FString& FileName,  bool bIsLoadingCloudSave);
	bool ShouldiCloudOverride(const UPickleballSaveGame* LocalSaveGame, const UPickleballSaveGame* CloudSaveGame) const;
	
	// Saving
	virtual void SaveGameData() override;
	virtual FPlayerData GetSaveGamePlayerData() override;
	virtual void SavePlayerData(FPlayerData PlayerData) override;
	virtual void SaveCurrentEnemyRow(int32 EnemyRow) override;
	void SaveVolumes(float MasterVolume, float MusicVolume, float SFXVolume);
	
	// Getters
	UFUNCTION(BlueprintCallable)
	bool GetIsFirstTimePlaying() const;
	UFUNCTION(BlueprintCallable)
	bool GetShouldLaunchStarterScreen() const;
	UFUNCTION()
	bool GetIsFirstTimePlayingInSession() const;
	UFUNCTION()
	bool GetIsGameLoaded() const;
	UFUNCTION()
	TArray<float> GetSaveGameVolumes();
	UFUNCTION()
	int32 GetSaveGameEnemyRow();
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentGameCount() const;

	// Setters
	UFUNCTION()
	void SetShouldLaunchStarterScreen(bool bIShouldLaunchStarterScreenIn);
	UFUNCTION()
	void SetIsFirstTimePlayingEver(bool bIsFirstTimePlayingEverIn);
    UFUNCTION()
	void SetIsFirstTimePlayingInSession(bool bIsFirstTimePlayingInSessionIn);

	// Ads
    UFUNCTION(BlueprintCallable)
    void RemoveAds();
    UFUNCTION(BlueprintCallable)
    bool AreAdsEnabled() const;

	// Misc.
	UFUNCTION(BlueprintCallable)
	void IncrementCurrentGameCount();

	// Delegates
	UFUNCTION(BlueprintCallable)
	void RewardFinishedComplete() const;
	
	UPROPERTY()
	FLoadFinishedDelegate LoadFinished;
	
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAdsRemovedDelegate AdsRemoved;
	
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAdButtonPressed OnAdButtonPressed;
	
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRewardFinished RewardFinished;
	
	FOnLoginCompleteDelegate OnLoginCompleteDelegate;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	UPickleballSaveGame* SaveGame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	FString SlotName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	FString CurrentUserId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save, meta = (AllowPrivateAccess))
	bool bIsFirstTimePlayingEver;

	UPROPERTY()
	bool bIsFirstTimePlayingInSession;

	UPROPERTY()
	bool bShouldLaunchStarterScreen;

	UPROPERTY()
	bool bIsFirstHomeScreenLaunch;

	UPROPERTY()
    bool bIsGameLoaded;

	bool bIsLoggedIn;
    
    int32 CurrentGameCount;
};

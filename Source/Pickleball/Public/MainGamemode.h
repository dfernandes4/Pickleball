// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGamemode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdated, int, NewScore);

UCLASS()
class PICKLEBALL_API AMainGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGamemode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* MasterSoundClass;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* SFXSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundClass* MusicSoundClass;


	FOnScoreUpdated OnScoreUpdated;
};

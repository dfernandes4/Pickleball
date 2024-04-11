// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PickleBallGameInstance.generated.h"


class UHomeScreenWidget;
/**
 * 
 */
UCLASS()
class PICKLEBALL_API UPickleBallGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPickleBallGameInstance();
	virtual void Init() override;
};

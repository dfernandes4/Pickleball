// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "PaddleAbility.generated.h"

UENUM(BlueprintType, Meta = (ScriptName="PaddleAbilityEnum"))
enum class EPaddleAbility : uint8
{
	None,
	CoinMultiplier
};

UCLASS(BlueprintType, Meta = (ScriptName="PaddleAbilityObject"))
class PICKLEBALL_API UPaddleAbility : public UUserDefinedEnum
{
	GENERATED_BODY()

	UPaddleAbility();
};

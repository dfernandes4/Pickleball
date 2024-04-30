// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "PaddleRarity.generated.h"

UENUM(BlueprintType, Meta = (ScriptName="PaddleRarityEnum"))
enum class EPaddleRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary
};

UCLASS(BlueprintType, Meta = (ScriptName="PaddleRarityObject"))
class PICKLEBALL_API UPaddleRarity : public UUserDefinedEnum
{
	GENERATED_BODY()

	UPaddleRarity();
};

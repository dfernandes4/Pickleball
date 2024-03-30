// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "AIState.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Responding
};

UCLASS(BlueprintType)
class PICKLEBALL_API UAIState : public UUserDefinedEnum
{
	GENERATED_BODY()

	UAIState();
};

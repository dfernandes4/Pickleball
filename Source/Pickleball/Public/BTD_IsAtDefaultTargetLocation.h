﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsAtDefaultTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UBTD_IsAtDefaultTargetLocation : public UBTDecorator
{
	GENERATED_BODY()

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

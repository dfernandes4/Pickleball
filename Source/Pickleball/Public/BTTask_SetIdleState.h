// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetIdleState.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API UBTTask_SetIdleState : public UBTTaskNode
{
	GENERATED_BODY()
public:
	
	UBTTask_SetIdleState();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};

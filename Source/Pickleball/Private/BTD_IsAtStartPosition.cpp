// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsAtDefaultTargetLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_IsAtDefaultTargetLocation::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const FVector DefaultTargetLocation = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsVector("DefaultTargetLocationKey");
	return OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation().Equals(DefaultTargetLocation);
}

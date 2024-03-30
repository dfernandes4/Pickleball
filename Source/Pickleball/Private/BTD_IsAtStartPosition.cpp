// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsAtStartPosition.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_IsAtStartPosition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const FVector StartLocation = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsVector("StartLocationKey");
	return OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation().Equals(StartLocation);
}

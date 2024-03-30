// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsInHittingZone.h"
#include "AIController.h"
#include "EnemyPaddle.h"

bool UBTD_IsInHittingZone::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const TObjectPtr<AEnemyPaddle> EnemyPaddle {Cast<AEnemyPaddle>(OwnerComp.GetAIOwner()->GetPawn())};
	if(IsValid(EnemyPaddle.Get()))
	{
		return EnemyPaddle->GetIsInHittingZone();
	}
	return true;
}

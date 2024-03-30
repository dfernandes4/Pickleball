// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetIdleState.h"

#include "AIState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetIdleState::UBTTask_SetIdleState()
{
	NodeName = TEXT("Set Idle State");
}

EBTNodeResult::Type UBTTask_SetIdleState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIStateKey", static_cast<uint8>(EAIState::Idle));
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_SetIdleState::GetStaticDescription() const
{
	return FString("Set the AI to idle state");
}

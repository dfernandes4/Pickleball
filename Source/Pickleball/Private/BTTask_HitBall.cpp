 // Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HitBall.h"

#include "NavigationSystem.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"
#include "EnemyPaddle.h"
#include "BehaviorTree/BlackboardComponent.h"

 UBTTask_HitBall::UBTTask_HitBall()
 {
 	NodeName = TEXT("Hit Ball");
 }

 EBTNodeResult::Type UBTTask_HitBall::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
 {
	Cast<AEnemyPaddle>(OwnerComp.GetAIOwner()->GetPawn())->HitBall();
 	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
 }

 FString UBTTask_HitBall::GetStaticDescription() const
 {
  return FString("Hits Ball when in range of the ball.");
 }

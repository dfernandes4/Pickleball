


#include "BTD_DidBallLandInCourt.h"

#include "AIController.h"
#include "EnemyAIController.h"
#include "EnemyPaddle.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_DidBallLandInCourt::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
uint8* NodeMemory) const
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	const FVector BallLandingLocation = EnemyAIController->GetBlackboardComponent()->GetValueAsVector("BallLandingLocationKey");
	if(BallLandingLocation.X > 20 && BallLandingLocation.X < 680 && BallLandingLocation.Y > -313 && BallLandingLocation.Y < 313 || Cast<AEnemyPaddle>(EnemyAIController->GetPawn())->GetIsFirstSwing())
	{
		return true;
	}

	return false;
}
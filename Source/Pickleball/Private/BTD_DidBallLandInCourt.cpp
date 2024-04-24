


#include "BTD_DidBallLandInCourt.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_DidBallLandInCourt::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
uint8* NodeMemory) const
{
	const FVector BallLandingLocation = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsVector("BallLandingLocationKey");
	if(BallLandingLocation.X > 20 && BallLandingLocation.X < 680 && BallLandingLocation.Y > -313 && BallLandingLocation.Y < 313)
	{
		return true;
	}

	return false;
}
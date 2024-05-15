
#include "EnemyAIController.h"

#include "AIState.h"
#include "Ball.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	
	AIStateKey = "AIStateKey";
	LocationToHitAtKey = "LocationToHitAtKey";
	DefaultTargetLocationKey = "DefaultTargetLocationKey";
	BallLandingLocationKey = "BallLandingLocationKey";
	IsInHittingZoneKey = "IsInHittingZoneKey";
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(Blackboard.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset.Get());

		// Find target point actor bp in scene named BP_DefaultTargetLocation

		ATargetPoint* DefaultTargetLocation = Cast<ATargetPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), ATargetPoint::StaticClass()));
		Blackboard->SetValueAsVector(DefaultTargetLocationKey, DefaultTargetLocation->GetActorLocation());
		Blackboard->SetValueAsVector(LocationToHitAtKey, (UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass())->GetActorLocation() + FVector(80.f, 0.f, 0.f)));
		
		Blackboard->SetValueAsEnum(AIStateKey, static_cast<uint8>(EAIState::Responding));
		Blackboard->SetValueAsBool(IsInHittingZoneKey, false);
	}
}

void AEnemyAIController::StartBehaviorTree() const
{
	if (BehaviorTree.Get() != nullptr)
	{
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void AEnemyAIController::SetRespondingState() const
{
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<uint8>(EAIState::Responding));
}

void AEnemyAIController::SetRespondingState(const FVector& LocationToHitAt) const
{
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<uint8>(EAIState::Responding));
	Blackboard->SetValueAsVector(LocationToHitAtKey, LocationToHitAt);
}

void AEnemyAIController::SetIdleState() const
{
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<uint8>(EAIState::Idle));
}

void AEnemyAIController::SetBallLandingLocation(const FVector& BallLandingLocation) const
{
	Blackboard->SetValueAsVector(BallLandingLocationKey, BallLandingLocation);
}

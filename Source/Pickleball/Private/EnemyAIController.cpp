
#include "EnemyAIController.h"

#include "AIState.h"
#include "Ball.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	
	AIStateKey = "AIStateKey";
	LocationToHitAtKey = "LocationToHitAtKey";
	StartLocationKey = "StartLocationKey";
	BallLandingLocationKey = "BallLandingLocationKey";
	IsInHittingZoneKey = "IsInHittingZoneKey";
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(Blackboard.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset.Get());

		Blackboard->SetValueAsVector(StartLocationKey, GetPawn()->GetActorLocation());
		Blackboard->SetValueAsVector(LocationToHitAtKey, (UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass())->GetActorLocation() + FVector(80.f, 0.f, 0.f)));
		
		Blackboard->SetValueAsEnum(AIStateKey, static_cast<uint8>(EAIState::Idle));
		Blackboard->SetValueAsBool(IsInHittingZoneKey, false);
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
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

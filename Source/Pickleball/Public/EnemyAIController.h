﻿#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AEnemyAIController();
	
	virtual void BeginPlay() override;
	void SetRespondingState() const;
	void SetRespondingState(const FVector& LocationToHitAt) const;
	void SetIdleState() const;
	void SetBallLandingLocation(const FVector& BallLandingLocation) const;
	void StartBehaviorTree() const;

private:
	
	// AI Components
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;


#pragma region BlackboardKeys

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BlackboardKeys", meta = (AllowPrivateAccess = "true"))
	FName AIStateKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BlackboardKeys", meta = (AllowPrivateAccess = "true"))
	FName LocationToHitAtKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BlackboardKeys", meta = (AllowPrivateAccess = "true"))
	FName DefaultTargetLocationKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BlackboardKeys", meta = (AllowPrivateAccess = "true"))
	FName BallLandingLocationKey;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BlackboardKeys", meta = (AllowPrivateAccess = "true"))
	FName IsInHittingZoneKey;
	
	#pragma endregion BlackboardKeys
};

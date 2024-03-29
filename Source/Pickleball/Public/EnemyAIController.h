#pragma once

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
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

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
	
};

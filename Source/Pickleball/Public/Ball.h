// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "Ball.generated.h"

struct FPredictProjectilePathPointData;
class APaddle;
class AEnemyPaddle;
class APlayerPaddle;
class ABallPositionSymbol;
class USphereComponent;

UCLASS()
class PICKLEBALL_API ABall : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	ABall();

	UFUNCTION()
	void ApplySwipeForce(const FVector& Force, const APaddle* PaddleActor);

	UFUNCTION()
	void OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void PredictProjectileLandingPoint();

	UFUNCTION()
	void OnSwipeForceApplied(const FVector& HittingLocation);

	/*UFUNCTION()
	FVector FindHittingLocation(bool bIsPlayerPaddle, const FVector& BallsVelocity, const TArray<FPredictProjectilePathPointData>& PathData) const;*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	USphereComponent* BallCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = components, meta = (AllowPrivateAccess))
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = components)
	USoundBase* HitSound;
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category = BaseVariables, meta = (AllowPrivateAccess))
	int32 Speed;

	UPROPERTY(VisibleAnywhere, Category = BaseVariables, meta = (AllowPrivateAccess))
	ABallPositionSymbol* BallPositionSymbol;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess))
	APaddle* CurrentPaddle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess))
	APlayerPaddle* PlayerPaddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess))
	AEnemyPaddle* EnemyPaddle;

	UPROPERTY()
	bool bDidBallLand;

	UPROPERTY()
	float BallLandingZ;
	
};

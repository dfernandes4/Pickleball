// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class USphereComponent;

UCLASS()
class PICKLEBALL_API ABall : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	ABall();

	UFUNCTION()
	void ApplySwipeForce(const FVector& Force) const;

	UFUNCTION()
	void OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	USphereComponent* BallCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UStaticMeshComponent* BallMesh;
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = BaseVariables, meta = (AllowPrivateAccess))
	int32 Speed;
};
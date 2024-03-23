// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paddle.generated.h"

class UPaperSpriteComponent;
class UBoxComponent;

UCLASS()
class PICKLEBALL_API APaddle : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaddle();

	UFUNCTION()
	void Swing(float SwipeLength, const FVector& SwipeDirection, float SwipeTime);


	UFUNCTION()
	void OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UPaperSpriteComponent* PaddleSprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components ,meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;
	

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Swiping, meta = (AllowPrivateAccess))
	float ForceOfPaddle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	FVector CurrentSwipeDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	float CurrentSwipeLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	bool bIsSwingActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	float CurrentSwipeTime;

	
	const float MinForce = 10.0f;
	const float MaxForce = 200.0f;
	

protected:
	
	virtual void BeginPlay() override;

};

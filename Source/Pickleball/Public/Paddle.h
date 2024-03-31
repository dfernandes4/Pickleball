// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paddle.generated.h"

class UArrowComponent;
class ABall;
class UPaperSpriteComponent;
class UBoxComponent;

UCLASS()
class PICKLEBALL_API APaddle : public APawn
{
	GENERATED_BODY()
	
public:
	
	APaddle();
	
	UFUNCTION()
	void OnPaddleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPaddleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	bool GetIsInHittingZone() const;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UPaperSpriteComponent* PaddleSprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components ,meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	bool bIsInHittingZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Swiping, meta = (AllowPrivateAccess))
	ABall* BallInScene;

	

	

	virtual void BeginPlay() override;

};

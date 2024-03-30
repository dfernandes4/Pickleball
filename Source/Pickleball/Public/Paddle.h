// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paddle.generated.h"

class ABall;
class UPaperSpriteComponent;
class UBoxComponent;

UCLASS()
class PICKLEBALL_API APaddle : public APawn
{
	GENERATED_BODY()
	
public:
	
	APaddle();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UPaperSpriteComponent* PaddleSprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components ,meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;

};

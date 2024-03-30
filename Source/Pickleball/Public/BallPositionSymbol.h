// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Components/ArrowComponent.h"
#include "BallPositionSymbol.generated.h"

UCLASS()
class PICKLEBALL_API ABallPositionSymbol : public AActor
{
	GENERATED_BODY()
	
public:    

	ABallPositionSymbol();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;
	
	// PaperSpriteComponent to display the 2D sprite
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = "true"))
	UArrowComponent* ArrowComponent;
};
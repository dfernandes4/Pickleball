// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "BackgroundFloor.generated.h"

/**
 * 
 */
UCLASS()
class PICKLEBALL_API ABackgroundFloor : public AActor
{
	GENERATED_BODY()
	
public:
    
    ABackgroundFloor();
    
    UFUNCTION()
    void SetNewMaterial(UMaterial* NewMaterial);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
    USceneComponent* SceneComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
    UStaticMeshComponent* StaticMeshComponent;
};

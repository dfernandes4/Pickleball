// Fill out your copyright notice in the Description page of Project Settings.


#include "BackgroundFloor.h"

ABackgroundFloor::ABackgroundFloor()
{
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;
    
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(SceneComponent);
}

void ABackgroundFloor::SetNewMaterial(UMaterial* NewMaterial)
{
    if(NewMaterial != nullptr)
    {
        StaticMeshComponent->SetMaterial(0, NewMaterial);
    }
}

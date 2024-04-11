// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGamemode.h"

#include "UserWidgetLoader.h"

void AMainGamemode::BeginPlay()
{
	Super::BeginPlay();

	const TObjectPtr<UWidgetLoader> WidgetLoader = NewObject<UWidgetLoader>(this);
	WidgetLoader->LoadWidget(FName("HomeScreen"), GetWorld());
}

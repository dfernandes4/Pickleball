﻿#include "UserWidgetLoader.h"

#include "WidgetClassInfo.h"
#include "Blueprint/UserWidget.h"

UWidgetLoader::UWidgetLoader()
{
	const ConstructorHelpers::FObjectFinder<UDataTable> WidgetClassTableFinder
		(TEXT("/Script/Engine.DataTable'/Game/Blueprints/Datatables/DT_WidgetClassInfo.DT_WidgetClassInfo'"));
	           
	if(WidgetClassTableFinder.Succeeded())
	{
		WidgetClassTable = WidgetClassTableFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Widget class table not found"));
		WidgetClassTable = nullptr;
	}
}

UUserWidget* UWidgetLoader::LoadWidget(const FName& WidgetName,  UWorld* WorldContextObject, int32 PreferredZOrder) const
{
	if(WidgetClassTable != nullptr)
	{
		const TSubclassOf<UUserWidget> WidgetClass =  WidgetClassTable->FindRow<FWidgetClassInfo>
													        (WidgetName, FString(""))->WidgetClass;

		if(IsValid(WidgetClass))
		{
			UUserWidget* WidgetObject = CreateWidget<UUserWidget>(WorldContextObject, WidgetClass);
			if(WidgetObject != nullptr)
			{
				WidgetObject->AddToViewport(PreferredZOrder);
				
				return WidgetObject;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Widget class table not found"));
	}
	return nullptr;
}

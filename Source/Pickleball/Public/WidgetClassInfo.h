#pragma once

#include "CoreMinimal.h"
#include "WidgetClassInfo.generated.h"

USTRUCT(BlueprintType)
struct FWidgetClassInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetClass;
};

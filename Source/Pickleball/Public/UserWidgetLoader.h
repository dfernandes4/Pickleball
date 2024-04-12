#pragma once
#include "CoreMinimal.h"
#include "UserWidgetLoader.generated.h"

UCLASS()
class PICKLEBALL_API UWidgetLoader : public UObject
{
public:
	GENERATED_BODY()
	
	UWidgetLoader();

	UFUNCTION()
	UUserWidget* LoadWidget(const FName& WidgetName, UWorld* WorldContextObject, int32 PreferredZOrder = 0) const;
	
private:
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WidgetClassTable;
};

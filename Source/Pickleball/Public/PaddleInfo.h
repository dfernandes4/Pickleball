#pragma once

#include "CoreMinimal.h"
#include "PaddleRarity.h"
#include "PaddleInfo.generated.h"

USTRUCT(BlueprintType)
struct FPaddleInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* PaddleTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPaddleRarity Rarity;
};

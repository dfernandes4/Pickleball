#pragma once

#include "CoreMinimal.h"
#include "PaddleAbility.h"
#include "PaddleRarity.h"
#include "PaddleInfo.generated.h"

class UPaperSprite;

USTRUCT(BlueprintType)
struct FPaddleInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSprite> PaddleSprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> PaddleSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPaddleAbility PaddleAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPaddleRarity PaddleRarity;
};

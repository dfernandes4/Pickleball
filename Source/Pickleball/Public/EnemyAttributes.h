#pragma once

#include "CoreMinimal.h"
#include "EnemyAttributes.generated.h"

class UPaperSprite;

USTRUCT(BlueprintType)
struct FEnemyAttributes : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterial> BackgroundMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPaperSprite> PaddleSprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> PaddleSoundEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> EnemyIconTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString  EnemyName;

	
	
};

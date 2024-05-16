#pragma once

#include "CoreMinimal.h"
#include "PlayerData.h"
#include "UObject/Interface.h"
#include "SaveGameInterface.generated.h"

class UPickleballSaveGame;

UINTERFACE(MinimalAPI)
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

class ISaveGameInterface
{
	GENERATED_BODY()

public:
	
	virtual void LoadGameData() = 0;
	virtual void SaveGameData() = 0;
	virtual FPlayerData GetSaveGamePlayerData() = 0;
	virtual void SavePlayerData(FPlayerData) = 0;
};
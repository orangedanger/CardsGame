// Future do it

#pragma once

#include "CoreMinimal.h"
#include "Cards/Data/TileDataAsset.h"
#include "GameFramework/GameMode.h"
#include "CardsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CARDS_API ACardsGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTileDataAsset> TileDataAsset;
};

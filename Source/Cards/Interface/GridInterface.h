// Future do it

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Cards/Data/TileDataAsset.h"
#include "GridInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGridInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CARDS_API IGridInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	//当需要更改单个或个别少量Tile时调用此函数
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void UpdateInstance(FTileInfo Info);
};

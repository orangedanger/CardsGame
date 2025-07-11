// Future do it

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISMGridSyncBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class CARDS_API UISMGridSyncBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ISM Grid Sync Blueprints",meta = (DefaultToSelf = "WorldContext"))
	static FVector SnapToGrid(UObject* WorldContext,const FVector& StartCenter, const FVector& GridSize);
};

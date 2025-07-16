// Future do it

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ISMGridSyncBlueprintLibrary.generated.h"

class UDebugWidgetController;
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class CARDS_API UISMGridSyncBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ISM Grid Sync Blueprints",meta = (DefaultToSelf = "WorldContext"))
	static FVector SnapToGrid(const UObject* WorldContext,const FVector& StartCenter, const FVector& GridSize);
	
	UFUNCTION(BlueprintPure, Category = "ISM Grid Sync Blueprints",meta = (DefaultToSelf = "WorldContext"))
	static UDebugWidgetController* GetDebugWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "ISM Grid Sync Blueprints",meta = (DefaultToSelf = "WorldContext"))
	static FHitResult GetCursorHitTile(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "ISM Grid Sync Blueprints",meta = (DefaultToSelf = "WorldContext"))
	static FVector GetCursorHitTileLocation(const UObject* WorldContext,const FVector& GridsCenter);

	UFUNCTION(BlueprintPure, Category = "ISM Grid Sync Blueprints",meta = (DefaultToSelf = "WorldContext"))
	static FIntPoint GetIndexFromWorldLocation(const UObject* WorldContext,const FVector& WorldLocation,const AGridsActor* Grids);
};

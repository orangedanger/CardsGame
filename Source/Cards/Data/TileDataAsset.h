// Future do it

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileDataAsset.generated.h"

UENUM(BlueprintType)
enum class ETileShape : uint8
{
	None,
	//正方形
	Square,
	//三角形
	Triangle,
	//六边形
	Hexagon		
};

USTRUCT()
struct FTileTypeSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UMaterialInterface* TileMaterial = nullptr;
	UPROPERTY(EditAnywhere)
	UStaticMesh* TileStaticMesh = nullptr;
};


/**
 * 
 */
UCLASS()
class CARDS_API UTileDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<ETileShape, FTileTypeSetting> TileShapes;

	
	
};

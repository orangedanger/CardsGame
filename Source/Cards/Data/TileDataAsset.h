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
	UPROPERTY(EditAnywhere)
	UMaterialInterface* HoveredMaterial = nullptr;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* PressedMaterial = nullptr;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* DisabledMaterial = nullptr;
};

UENUM(BlueprintType)
enum class ETileState : uint8
{
	Normal,
	Hovered,
	Pressed,
	Disabled,
	None	
};

USTRUCT(BlueprintType,Blueprintable)
struct FTileInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	ETileShape TileShape = ETileShape::None ;	//形状
	
	UPROPERTY(BlueprintReadWrite)
	FTransform Transform = FTransform();	//位置旋转缩放信息
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ETileState>TileStates = TArray<ETileState>(); //Tiles的状态信息

	UPROPERTY(BlueprintReadWrite)
	FIntPoint Index = FIntPoint();			//Tile的Index信息
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

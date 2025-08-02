// Future do it

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cards/Data/TileDataAsset.h"

#include "GridsActor.generated.h"

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
	UPROPERTY(BlueprintReadOnly)
	ETileShape TileShape = ETileShape::None ;	//形状
	
	UPROPERTY(BlueprintReadOnly)
	FTransform Transform = FTransform();	//位置旋转缩放信息
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ETileState>TileStates = TArray<ETileState>(); //Tiles的状态信息

	UPROPERTY(BlueprintReadOnly)
	FIntPoint Index = FIntPoint();			//Tile的Index信息
};

class UStaticMeshComponent;

UCLASS()
class CARDS_API AGridsActor : public AActor
{
private:
	GENERATED_BODY()

public:	
	AGridsActor();

	UFUNCTION(BlueprintCallable)
	void DrawGrid();

	UFUNCTION(BlueprintCallable)
	void ClearGrid();

	//当需要更改全部Tile时调用此函数
	UFUNCTION(BlueprintCallable)
	void UpdateGrid();
	
	UFUNCTION(BlueprintCallable)
	void SetTileShape(ETileShape shapeType);
	
	UFUNCTION(BlueprintCallable)
	void AddTileState(ETileState TileState);
	
	UFUNCTION(BlueprintCallable)
	void RemoveTileState(ETileState TileState);

	UFUNCTION(BlueprintCallable)
	void AddInstance(FIntPoint Index,FTransform Transform);
	
	UFUNCTION(BlueprintCallable)
	void RemoveInstance(FIntPoint Index) const;

	//当需要更改单个或个别少量Tile时调用此函数
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void UpdateInstance(const FTileInfo& Info);
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	UInstancedStaticMeshComponent* Mesh;

	
	FVector LeftBottomLocation;
	TMap<FIntPoint,FTileInfo> Tiles;
	/**
	 * 一些在UE中设置的变量
	 **/
	UPROPERTY(EditAnywhere,Category="Settings")
	FVector CenterLocation;
	UPROPERTY(EditAnywhere,Category="Settings")
	FVector GridSize;
	UPROPERTY(EditAnywhere,Category="Settings")
	FVector2D CountSize;
	UPROPERTY(EditAnywhere,Category="Settings")
	ETileShape ShapeType;
	
private:
	// 初始化网格
	void InitializeGrid();
	
	void HexagonCountSet(int32& Length, int32& Width);
	static bool IsWalkable(TArray<FHitResult> Hits);
	FVector ProcessLeftBottomLocation(const int32 Length ,const int32 Width) const;
	void CalculateGridPositionAndRotation(
		FTransform &OutTransform,
		const bool IsLengthEven,
		const bool IsWidthEven,
		const double LengthOffSet,
		const double WidthOffset) const;

public:
	UFUNCTION(BlueprintCallable)
	TMap<FIntPoint,FTileInfo> GetTiles() const {return Tiles;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetCenterLocation() const{return CenterLocation;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetGridSize() const{return GridSize;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector2D GetCountSize() const{return CountSize;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETileShape GetShapeType() const{return ShapeType;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetLeftBottomLocation() const{return LeftBottomLocation;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetGridSize(const FVector& InGridSize){this->GridSize = InGridSize;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCountSize(const FVector2D& InCountSize){this->CountSize = InCountSize;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetShapeType(const ETileShape InShapeType){this->ShapeType = InShapeType;}
};

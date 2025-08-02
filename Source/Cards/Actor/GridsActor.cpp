// Future do it


#include "GridsActor.h"

#include "RegionalActor.h"
#include "Cards/BlueprintLibrary/ISMGridSyncBlueprintLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"
#include "Cards/Cards.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"

AGridsActor::AGridsActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
}

void AGridsActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeGrid();
}

void AGridsActor::InitializeGrid()
{
	SetTileShape(ShapeType);
	UpdateGrid();

	//根据需求设置数量（如4可存储RGB颜色和是否显示中间）
	Mesh->SetNumCustomDataFloats(4);
}

void AGridsActor::DrawGrid()
{
	//更新CenterLocation
	CenterLocation= GetActorLocation();
	
	//同步到网格上
	CenterLocation = UISMGridSyncBlueprintLibrary::SnapToGrid(CenterLocation ,GridSize);

	//四舍五入长和宽
	int32 Length =FMath::RoundToInt32(CountSize.X);
	int32 Width =FMath::RoundToInt32(CountSize.Y);
	
	//计算最左端点
	LeftBottomLocation = ProcessLeftBottomLocation(Length ,Width);

	//设置Tiles的缩放
	FTransform Transform = FTransform();
	FVector Scale = GridSize /Mesh->GetStaticMesh()->GetBoundingBox().GetSize();
	Scale.Z =1;
	Transform.SetScale3D(Scale);
	
	//由于我们使用的是双坐标 宽度
	//TODO:后期需要进行修改
	if (ShapeType == ETileShape::Hexagon)
	{
		HexagonCountSet(Length, Width);
	}
	
	//遍历生成每个网格
	for (int i = 0; i < Length; i++)
	{
		//判断长度是否为偶数
		for (int j = 0; j < Width; j++)
		{
			//声明当前位置避免重复调用get函数
			FVector CurrentPosition = Transform.GetLocation();
			
			//对碰撞的变量进行初始化
			TArray<FHitResult> Hits;
			const float Radius = GridSize.X/3;
			FVector Start(CurrentPosition.X, CurrentPosition.Y, CurrentPosition.Z + 500);
			FVector End(CurrentPosition.X, CurrentPosition.Y, CurrentPosition.Z - 500);
			
			//进行碰撞检测
			UKismetSystemLibrary::SphereTraceMulti(
				this,
				Start,
				End,
				Radius,
				UEngineTypes::ConvertToTraceType(TRACE_GROUND),
				false,
				TArray<AActor*>(),
				EDrawDebugTrace::None,
				Hits,
				true);
			
			if (Hits.Num()>0 && IsWalkable(Hits))
			{
				FIntPoint intPoint = FIntPoint(i,j);
				
				AddInstance(intPoint,Transform);
				
				//设置Tile的信息
				FTileInfo TileInfo;
				TileInfo.TileShape = ShapeType;
				TileInfo.Transform = Transform;
				TileInfo.TileStates.AddUnique(ETileState::Normal);
				//这里是将数据转化为Index 和 相对Location
				//成功则添加Tiles
				Transform.SetLocation(CurrentPosition - LeftBottomLocation);
				//六边形为双坐标宽度的
				if (ShapeType == ETileShape::Hexagon)
				{
					const int32 OffSetX = j%2==0?1:0; 
					intPoint.X *= 2;
					intPoint.X +=OffSetX;
				}
				//添加Tile到数组
				Tiles.Add(intPoint,TileInfo);
			}
		}
	}
}

void AGridsActor::AddInstance(FIntPoint Index,FTransform Transform)
{
	//判断在Index位置的 XY 奇偶 来决定 三角形和六边形偏移
	bool IsLengthEven = Index.X % 2 == 0;
	bool IsWidthEven  =	Index.Y % 2 == 0;
	double LengthOffSet = Index.X * GridSize.X;;
	double WidthOffset = Index.Y * GridSize.Y;
	
	//计算不同形状的Tile的偏移,得到Transform
	CalculateGridPositionAndRotation(Transform,IsLengthEven, IsWidthEven, LengthOffSet, WidthOffset);
	
	//设置bWorldSpace,Mesh中的坐标是世界坐标
	Mesh->AddInstance(Transform,true);
}

void AGridsActor::RemoveInstance(FIntPoint Index) const
{
	Mesh->RemoveInstance(Index.X * CountSize.X + Index.Y);
}

void AGridsActor::UpdateInstance_Implementation(const FTileInfo& Info)
{
	RemoveInstance(Info.Index);
	AddInstance(Info.Index,Info.Transform);

	//获取颜色信息
	//在蓝图中实现
}

void AGridsActor::ClearGrid()
{
	Mesh->ClearInstances();
	Tiles.Empty();
}

void AGridsActor::UpdateGrid()
{
	ClearGrid();
	DrawGrid();
}

void AGridsActor::SetTileShape(ETileShape shapeType)
{
	ShapeType = shapeType;
	if (shapeType == ETileShape::None)
	{
		Mesh->SetStaticMesh(nullptr);
		Mesh->SetMaterial(0, nullptr);
	}
	else
	{
		const UTileDataAsset* TileDataAsset =  UISMGridSyncBlueprintLibrary::GetTileDataAsset(this);
		const FTileTypeSetting* TileTypeSetting=TileDataAsset->TileShapes.Find(shapeType);
	
		Mesh->SetStaticMesh(TileTypeSetting->TileStaticMesh);
		Mesh->SetMaterial(0, TileTypeSetting->TileMaterial);
	}
}

void AGridsActor::AddTileState(ETileState TileState)
{
	const FVector HitTileLocation = UISMGridSyncBlueprintLibrary::GetCursorHitTileLocation(this,CenterLocation);
	const FIntPoint Index = UISMGridSyncBlueprintLibrary::GetIndexFromWorldLocation(HitTileLocation,this);

	if (FTileInfo* TileInfo =Tiles.Find(Index))
	{
		//检测TileStates有没有数据
		if (TileInfo->TileStates.AddUnique(TileState) >= 0)
		{
			//有数据就更新
			Tiles.Add(Index,*TileInfo);
			//更新单个Instance
			UpdateInstance(*TileInfo);
		}
	}
	
}

void AGridsActor::RemoveTileState(const ETileState TileState)
{
	const FVector HitTileLocation = UISMGridSyncBlueprintLibrary::GetCursorHitTileLocation(this,CenterLocation);
	const FIntPoint Index = UISMGridSyncBlueprintLibrary::GetIndexFromWorldLocation(HitTileLocation,this);

	if (FTileInfo* TileInfo =Tiles.Find(Index))
	{
		TileInfo->TileStates.Remove(TileState);
		Tiles.Add(Index,*TileInfo);
		
		//更新单个Instance
		UpdateInstance(*TileInfo);
	}
}

FVector AGridsActor::ProcessLeftBottomLocation(const int32 Length ,const int32 Width) const
{
	//当长度或宽度为奇数时 长度或宽度需要减去半个Size大小
	FVector Subtract = FVector(Length * GridSize.X/2,Width * GridSize.Y/2,0);
	if(Length%2 !=0)
	{
		Subtract.X -=GridSize.X/2;
	}
	if(Width%2 !=0)
	{
		Subtract.Y -=GridSize.Y/2;
	}
	return CenterLocation - Subtract;
}

bool AGridsActor::IsWalkable(TArray<FHitResult> Hits)
{
	bool bIsWalkable = true;
	for(auto& Hit : Hits)
	{
		if(ARegionalActor* RegionalActor =  Cast<ARegionalActor>(Hit.GetActor()))
		{
			ERegionalActor RegionalType = RegionalActor->GetRegionalType();
			if(RegionalType == ERegionalActor::Mountain || RegionalType == ERegionalActor::River ||RegionalType == ERegionalActor::Ocean)
			{
				bIsWalkable = false;
				break;
			}
		}
	}
	return bIsWalkable;
}

void AGridsActor::CalculateGridPositionAndRotation(FTransform &OutTransform,const bool IsLengthEven,const bool IsWidthEven,const double LengthOffSet,const double WidthOffset) const
{
	//不同形状的绘制不同
	FRotator OffsetRotation = FRotator::ZeroRotator;
	FVector OffsetLocation = FVector::ZeroVector;
	switch (ShapeType)
	{
	case ETileShape::None:
		break;
	case ETileShape::Square:
		//Grids的位置偏移
		OffsetLocation = FVector(LeftBottomLocation.X + LengthOffSet, LeftBottomLocation.Y + WidthOffset, LeftBottomLocation.Z);
		break;
	case ETileShape::Triangle:
		//Grids的位置偏移
		OffsetLocation = FVector(LeftBottomLocation.X + LengthOffSet, LeftBottomLocation.Y + WidthOffset/2, LeftBottomLocation.Z);
		//是偶数时翻转
		if (IsWidthEven)
		{
			OffsetRotation +=FRotator(0,180,0);
		}
		if (IsLengthEven)
		{
			OffsetRotation +=FRotator(0,180,0);
		}
		break;
	case ETileShape::Hexagon:
		//Grids的位置偏移
		OffsetLocation = FVector(LeftBottomLocation.X + LengthOffSet *1.5 , LeftBottomLocation.Y + WidthOffset/2, LeftBottomLocation.Z);
		if (IsWidthEven)
		{
			OffsetLocation += FVector(GridSize.X * 0.75, 0 ,0);
		}
		break;
	}
	OutTransform.SetRotation(OffsetRotation.Quaternion());
	OutTransform.SetLocation(OffsetLocation);
}

void AGridsActor::HexagonCountSet(int32& Length, int32& Width)
{
	Length /=2;
	Width*=2;
}



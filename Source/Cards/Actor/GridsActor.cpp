// Future do it


#include "GridsActor.h"

#include "RegionalActor.h"
#include "Cards/BlueprintLibrary/ISMGridSyncBlueprintLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"
#include "Cards/Cards.h"

AGridsActor::AGridsActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
}

void AGridsActor::DrawGrid()
{
	//更新CenterLocation
	CenterLocation= GetActorLocation();
	//同步到网格上
	CenterLocation = UISMGridSyncBlueprintLibrary::SnapToGrid(this,CenterLocation ,GridSize);

	//四舍五入长和宽
	const int32 Length =FMath::RoundToInt32(CountSize.X);
	const int32 Width =FMath::RoundToInt32(CountSize.Y);
	
	//计算最左端点
	LeftBottomLocation = ProcessLeftBottomLocation(Length ,Width);
	
	//设置每个的缩放和位置
	FTransform Transform = FTransform();
	FVector Scale = GridSize /Mesh->GetStaticMesh()->GetBoundingBox().GetSize();
	Scale.Z =1;
	Transform.SetScale3D(Scale);
	FVector StartLocation = LeftBottomLocation;
	//遍历生成每个网格
	for (int i = 0; i < Length; i++)
	{
		for (int j = 0; j < Width; j++)
		{

			//不同形状的绘制不同
			FRotator OffsetRotation = FRotator::ZeroRotator;
			switch (ShapeType)
			{
			case ETileShape::None:
				break;
			case ETileShape::Square:
				//Grids的位置偏移
				StartLocation = FVector(LeftBottomLocation.X + i* GridSize.X, LeftBottomLocation.Y + j*GridSize.Y, LeftBottomLocation.Z);
				break;
			case ETileShape::Triangle:
				//Grids的位置偏移
				StartLocation = FVector(LeftBottomLocation.X + i* GridSize.X, LeftBottomLocation.Y + j*GridSize.Y/2, LeftBottomLocation.Z);
				//是偶数时翻转
				if (j % 2 == 0)
				{
					OffsetRotation +=FRotator(0,180,0);
				}
				if (i %2 == 0)
				{
					OffsetRotation +=FRotator(0,180,0);
				}
				Transform.SetRotation(OffsetRotation.Quaternion());
				
				
				break;
			case ETileShape::Hexagon:
				//Grids的位置偏移
				StartLocation = FVector(LeftBottomLocation.X + i* GridSize.X *1.5 , LeftBottomLocation.Y + j*GridSize.Y/2, LeftBottomLocation.Z);
				if (j % 2 == 0)
				{
					StartLocation += FVector(GridSize.X * 0.75, 0 ,0);
				}

				break;
			}

			//对碰撞的变量进行初始化
			TArray<FHitResult> Hits;
			const float Radius = GridSize.X/3;
			FVector Start(StartLocation.X, StartLocation.Y, StartLocation.Z + 500);
			FVector End(StartLocation.X, StartLocation.Y, StartLocation.Z - 500);

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
			
			FIntPoint intPoint = FIntPoint(i,j);
			//检测是否碰撞来选择是否进行添加
			if (AddGridsToMesh(Transform, StartLocation, Hits, Radius))
			{
				//成功则添加Tiles
				Transform.SetLocation(StartLocation - LeftBottomLocation);
				Tiles.Add(FIntPoint(i,j),FTileInfo(ShapeType ,Transform));
			}
		}
	}
}

void AGridsActor::ClearGrid()
{
	Mesh->ClearInstances();
	Tiles.Empty();
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

bool AGridsActor::AddGridsToMesh(FTransform Transform, FVector& StartLocation, TArray<FHitResult> Hits, const float Radius) const
{
	//如果检测到碰撞且满足可以行走
	if(Hits.Num()>0 && IsWalkable(Hits))
	{
		constexpr float OffsetGroundValue = 1.f;
		StartLocation.Z = Hits[0].Location.Z  - Radius + OffsetGroundValue;
		Transform.SetLocation(StartLocation);
		//设置bWorldSpace
		Mesh->AddInstance(Transform,true);
		Transform.SetLocation(StartLocation - LeftBottomLocation);
		return true;
	}
	return false;
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

void AGridsActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetTileShape(ShapeType);
}


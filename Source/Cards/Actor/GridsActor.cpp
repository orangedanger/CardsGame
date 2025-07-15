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
			//Grids的位置偏移
			StartLocation = FVector(LeftBottomLocation.X + i* GridSize.X, LeftBottomLocation.Y + j*GridSize.Y, LeftBottomLocation.Z);
			
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
				static_cast<ETraceTypeQuery>(TRACE_GROUND),
				false,
				TArray<AActor*>(),
				EDrawDebugTrace::None,
				Hits,
				true);

			//检测是否碰撞来选择是否进行添加
			AddGridsToMesh(Transform, StartLocation, Hits, Radius);
		}
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

void AGridsActor::AddGridsToMesh(FTransform Transform, FVector& StartLocation, TArray<FHitResult> Hits, const float Radius) const
{
	//如果检测到碰撞且满足可以行走
	if(Hits.Num()>0 && IsWalkable(Hits))
	{
		constexpr float OffsetGroundValue = 1.f;
		StartLocation.Z = Hits[0].Location.Z  - Radius + OffsetGroundValue;
		Transform.SetLocation(StartLocation);
		//设置bWorldSpace
		Mesh->AddInstance(Transform,true);
	}
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

}


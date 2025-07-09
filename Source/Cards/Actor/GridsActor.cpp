// Future do it


#include "GridsActor.h"

#include "Cards/BlueprintLibrary/ISMGridSyncBlueprintLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"

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
	CenterLocation = UISMGridSyncBlueprintLibrary::SyncGridsVector(this,CenterLocation ,GridSize);

	//四舍五入长和宽
	const int32 Length =FMath::RoundToInt32(CountSize.X);
	const int32 Width =FMath::RoundToInt32(CountSize.Y);

	//当长度或宽度为奇数时 长度或宽度需要减去半个Size大小
	FVector Subtract = FVector(Length * GridSize.X/2,Width * GridSize.Y/2,-1);
	if(Length%2 !=0)
	{
		Subtract.X -=GridSize.X/2;
	}
	if(Width%2 !=0)
	{
		Subtract.Y -=GridSize.Y/2;
	}
	//计算最左端点
	LeftBottonLocation = CenterLocation -Subtract;
	
	//设置每个的缩放和位置
	FTransform Transform = FTransform();
	FVector Scale = GridSize /Mesh->GetStaticMesh()->GetBoundingBox().GetSize();
	Scale.Z =1;
	Transform.SetScale3D(Scale);
	FVector StartLocation = LeftBottonLocation;
	//遍历生成每个网格
	for (int i = 0; i < Length; i++)
	{
		StartLocation.X += GridSize.X;
		StartLocation.Y = LeftBottonLocation.Y;
		for (int j = 0; j < Width; j++)
		{
			
			StartLocation.Y += GridSize.Y;
			FVector Start(StartLocation.X, StartLocation.Y, StartLocation.Z + 500);
			FVector End(StartLocation.X, StartLocation.Y, StartLocation.Z - 500);
			TArray<FHitResult> Hits;

			const float Radius = GridSize.X/3;
			//进行碰撞检测
			UKismetSystemLibrary::SphereTraceMulti(
				this,
				Start,
				End,
				Radius,
				ETraceTypeQuery(ECC_GameTraceChannel1),
				false,
				TArray<AActor*>(),
				EDrawDebugTrace::None,
				Hits,
				true);

			//检测是否碰撞
			if(Hits.Num()>0)
			{
				StartLocation.Z = Hits[0].Location.Z  - Radius + 1;
				Transform.SetLocation(StartLocation);
				//设置bWorldSpace
				Mesh->AddInstance(Transform,true);
			}
			
		}
	}
}

void AGridsActor::BeginPlay()
{
	Super::BeginPlay();

}


// Future do it


#include "ISMGridSyncBlueprintLibrary.h"

#include "Cards/Cards.h"
#include "Cards/Actor/GridsActor.h"
#include "Cards/GameMode/CardsGameMode.h"
#include "Cards/PlayerController/CardsPlayerController.h"
#include "Cards/UI/HUD/DebugHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


FVector UISMGridSyncBlueprintLibrary::SnapToGrid(const FVector& StartCenter, const FVector& GridSize)
{
	//(Floor((Location + (Grid/(T)2)) / Grid) * Grid)
	const float X = UKismetMathLibrary::GridSnap_Float(StartCenter.X,GridSize.X);
	const float Y = UKismetMathLibrary::GridSnap_Float(StartCenter.Y,GridSize.Y);
	const float Z = UKismetMathLibrary::GridSnap_Float(StartCenter.Z,GridSize.Z);
	FVector Result;
	Result.X = X;
	Result.Y = Y;
	Result.Z = Z;
	//UE_LOG(LogTemp, Warning, TEXT("Result:(X,Y):%f,%f"), Result.X, Result.Y);
	//UE_LOG(LogTemp, Warning, TEXT("StartCenter:(X,Y):%f,%f"), StartCenter.X, StartCenter.Y);
	return Result;
}

UDebugWidgetController* UISMGridSyncBlueprintLibrary::GetDebugWidgetController(const UObject* WorldContext)
{
	if (const ACardsPlayerController* CardsPC = Cast<ACardsPlayerController>(UGameplayStatics::GetPlayerController(WorldContext,0)))
	{
		if (ADebugHUD* DebugHUD = Cast<ADebugHUD>(CardsPC->GetHUD()))
		{
			return DebugHUD->GetDebugWidgetController();
		}
	}
	return nullptr;
}

FHitResult UISMGridSyncBlueprintLibrary::GetCursorHitTile(const UObject* WorldContext)
{
	if (const ACardsPlayerController* CardsPC = Cast<ACardsPlayerController>(UGameplayStatics::GetPlayerController(WorldContext,0)))
	{
		FHitResult Hit;
		CardsPC->GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(TRACE_GRIDS),
		false,
		Hit
		);
		return Hit;
	}
	return FHitResult();
}

FVector UISMGridSyncBlueprintLibrary::GetCursorHitTileLocation(const UObject* WorldContext,const FVector& GridsCenter)
{
	FHitResult Hit = GetCursorHitTile(WorldContext);
	FVector HitLocation = FVector::ZeroVector;
	if (Hit.bBlockingHit)
	{
		//击中了
		HitLocation = Hit.Location;
	}
	else
	{
		if (const ACardsPlayerController* CardsPC = Cast<ACardsPlayerController>(UGameplayStatics::GetPlayerController(WorldContext,0)))
		{
			FVector StartLocation;
			FVector WorldDirection;
			//将2D的鼠标位置转换成3D的位置
			CardsPC->DeprojectMousePositionToWorld(StartLocation,WorldDirection);
			//设置一个远处的点
			WorldDirection *= 99999999;
			const FVector EndLocation = StartLocation + WorldDirection;
			
			//计算与Grids平面的交点
			HitLocation = FMath::LinePlaneIntersection(
				StartLocation,
				EndLocation,
				GridsCenter,
				FVector(0,0,1));
		}
	}
	return HitLocation;
}

FIntPoint UISMGridSyncBlueprintLibrary::GetIndexFromWorldLocation(const FVector& WorldLocation,
	const AGridsActor* Grids)
{

	//定义一些需要使用的数据
	FIntPoint Index =FIntPoint::ZeroValue;
	FVector SnapLocation = FVector::ZeroVector;
	FVector2D SizeVector2D = FVector2D::ZeroVector;
	FVector2D SnapVector2D = FVector2D::ZeroVector;
	FVector2D Result = FVector2D::ZeroVector;

	//转化一下数据到2D格式
	const FVector GridSize = Grids->GetGridSize();
	//本地坐标:相对网格左下角的坐标
	const FVector LocalLocation = WorldLocation - Grids->GetLeftBottomLocation();

	//需要根据Shape的不同采取不同的方法
	switch (Grids->GetShapeType())
	{
	case ETileShape::None:
		break;
	case ETileShape::Square:
		//对于正方形之间获取相对坐标然后除以GridSize即可
		SnapLocation= SnapToGrid(LocalLocation,GridSize);
		
		SizeVector2D = FVector2D(GridSize.X,GridSize.Y);
		SnapVector2D = FVector2D(SnapLocation.X,SnapLocation.Y);
		
		Result = SnapVector2D / SizeVector2D;
		Index =Result.IntPoint();
		break;
	case ETileShape::Triangle:
		
		//三角形处理
		//这里把三角形当中一半的正方形处理以快速运算
		SnapLocation = SnapToGrid(
			LocalLocation,
			GridSize*FVector(1,0.5,1)
			);
		
		SizeVector2D = FVector2D(GridSize.X,GridSize.Y / 2);
		
		SnapVector2D = FVector2D(SnapLocation.X,SnapLocation.Y);
		
		Result = SnapVector2D / SizeVector2D;
	
		Index =Result.IntPoint();
		break;
	case ETileShape::Hexagon:

		//六边形经过处理
		//GridSize.X表示的是六边形的垂直长度
		//GridSize.Y表示的是六边形的水平长度
		
		//LocalLocation对于Y上的乘2是因为我们采用的是 宽度的双坐标 需要对宽度乘以二做处理
		//GridSize。Y*0.25原因是由于
		
		//0.75 * GridSize.X 上下两个六边形中心的垂直长度
		//+FVector(25.f,0,0)的原因是由于Snap只会Snap到 -50 0 50，50倍数的点位
		//实际上X坐标需要的点位是-25 25 75 等点位

		//TODO: 我希望 0 ~ 50 Snap到 25   50 ~ 100 Snap 到 75
		//然而现在是 -25 ~ 25 Snap 0     25 ~ 75 Snap 50
		//Snap公式 (Floor((Location + (Grid/(T)2)) / Grid) * Grid)
		//关键点 Floor((Location + (Grid/(T)2)) 需要让它 原本是 (1<=X<2 ) == 1 ->	(0.5 <=X<1.5） ==	0.5
		
		
		SnapLocation = SnapToGrid(
			LocalLocation*FVector(1,2,1),
			GridSize* FVector(0.75,0.5,1)
			);
		
		SnapLocation.Y = FMath::CeilToInt(LocalLocation.Y / (GridSize.Y/2) )*GridSize.Y/2 - GridSize.Y/4;
			
		
		SizeVector2D = FVector2D(GridSize.X * 0.75 ,GridSize.Y*0.25);
		
		SnapVector2D = FVector2D(SnapLocation.X,SnapLocation.Y);

		Result = SnapVector2D / SizeVector2D;

		Index =Result.IntPoint();
		
		
		if (Index.X % 2 ==0)
		{
			const int Base = (Index.Y - 1) / 4;
			// 奇数参考值映射：1,3→1; 5,7→3; 9,11→5
			Index.Y =2 * Base + 1;
		}
		else
		{
			const int Base = (Index.Y - 3) / 4;
			// 偶数参考值映射：3,5→2; 7,9→4; 11,13→6
			Index.Y =2 * (Base + 1);
		}
		
		break;
	}

	
	return Index;
}

UTileDataAsset* UISMGridSyncBlueprintLibrary::GetTileDataAsset(const UObject* WorldContext)
{
	if (ACardsGameMode* CardsGameMode = Cast<ACardsGameMode>(UGameplayStatics::GetGameMode(WorldContext)))
	{
		return CardsGameMode->TileDataAsset;
	}
	return nullptr;
}
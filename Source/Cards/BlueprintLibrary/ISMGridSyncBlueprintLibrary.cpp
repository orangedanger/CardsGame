// Future do it


#include "ISMGridSyncBlueprintLibrary.h"

#include "Cards/Cards.h"
#include "Cards/PlayerController/CardsPlayerController.h"
#include "Cards/UI/HUD/DebugHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


FVector UISMGridSyncBlueprintLibrary::SnapToGrid(UObject* WorldContext,const FVector& StartCenter, const FVector& GridSize)
{
	const float X = UKismetMathLibrary::GridSnap_Float(StartCenter.X,GridSize.X);
	const float Y = UKismetMathLibrary::GridSnap_Float(StartCenter.Y,GridSize.Y);
	const float Z = UKismetMathLibrary::GridSnap_Float(StartCenter.Z,GridSize.Z);
	FVector Result;
	Result.X = X;
	Result.Y = Y;
	Result.Z = Z;
	UE_LOG(LogTemp, Warning, TEXT("Result:(X,Y):%f,%f"), Result.X, Result.Y);
	UE_LOG(LogTemp, Warning, TEXT("StartCenter:(X,Y):%f,%f"), StartCenter.X, StartCenter.Y);
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
		static_cast<ETraceTypeQuery>(TRACE_GRIDS),
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


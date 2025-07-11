// Future do it


#include "ISMGridSyncBlueprintLibrary.h"
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


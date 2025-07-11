// Future do it

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridsActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class CARDS_API AGridsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridsActor();

	UFUNCTION(BlueprintCallable)
	void DrawGrid();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	UInstancedStaticMeshComponent* Mesh;
	
	FVector LeftBottonLocation;

public:
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
	FVector CenterLocation;
    
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
	FVector GridSize;
    
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
	FVector2D CountSize;
private:
	
	void AddGridsToMesh(FTransform Transform, FVector& StartLocation, TArray<FHitResult> Hits, const float Radius) const;
	
	static bool IsWalkable(TArray<FHitResult> Hits);
	
	FVector ProcessLeftBottonLocation(const int32 Length ,const int32 Width) const;
};

// Future do it

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridsActor.generated.h"

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

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	UInstancedStaticMeshComponent* Mesh;
	
	FVector LeftBottomLocation;
	UPROPERTY(EditAnywhere,Category="Settings")
	FVector CenterLocation;
    
	UPROPERTY(EditAnywhere,Category="Settings")
	FVector GridSize;
    
	UPROPERTY(EditAnywhere,Category="Settings")
	FVector2D CountSize;
	
public:
	
	UFUNCTION(BlueprintCallable)
	FVector GetCenterLocation() const{return CenterLocation;}
	UFUNCTION(BlueprintCallable)
	FVector GetGridSize() const{return GridSize;}
	UFUNCTION(BlueprintCallable)
	FVector2D GetCountSize() const{return CountSize;}
	UFUNCTION(BlueprintCallable)
	void SetCenterLocation(const FVector& InCenterLocation){this->CenterLocation = InCenterLocation;}
	UFUNCTION(BlueprintCallable)
	void SetGridSize(const FVector& InGridSize){this->GridSize = InGridSize;}
	UFUNCTION(BlueprintCallable)
	void SetCountSize(const FVector2D& InCountSize){this->CountSize = InCountSize;}
private:
	void AddGridsToMesh(FTransform Transform, FVector& StartLocation, TArray<FHitResult> Hits, const float Radius) const;
	
	static bool IsWalkable(TArray<FHitResult> Hits);
	
	FVector ProcessLeftBottomLocation(const int32 Length ,const int32 Width) const;
};

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

	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
	FVector CenterLocation;

	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
	FVector GridSize;

	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true),Category="Settings")
	FVector2D CountSize;
	
	FVector LeftBottonLocation;
};

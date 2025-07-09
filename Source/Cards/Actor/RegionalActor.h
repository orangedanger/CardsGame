// Future do it

#pragma once

UENUM(BlueprintType)
enum class ERegionalActor : uint8
{
	Mountain,
	River,
	Ocean,
	Wasteland,
	Forest,
	SpawnPoint,
	Ground,
	None
};

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RegionalActor.generated.h"
class UStaticMeshComponent;

UCLASS()
class CARDS_API ARegionalActor : public AActor
{
	GENERATED_BODY()

public:
	ARegionalActor();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,Category="Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere,Category="RegionalType")
	ERegionalActor RegionalType = ERegionalActor::None;
};

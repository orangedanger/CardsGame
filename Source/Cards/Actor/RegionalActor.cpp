// Future do it


#include "RegionalActor.h"


ARegionalActor::ARegionalActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetHiddenInGame(true);
}


void ARegionalActor::BeginPlay()
{
	Super::BeginPlay();

	
	
}


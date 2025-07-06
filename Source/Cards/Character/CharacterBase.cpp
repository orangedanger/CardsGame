#include "CharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetRelativeLocation(FVector(0, 0, 0));
	CameraComponent->SetupAttachment(SphereComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->TargetArmLength=800;
	SpringArm->SetupAttachment(CameraComponent);
}


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


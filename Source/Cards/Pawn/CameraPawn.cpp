// Future do it


#include "CameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Misc/MapErrors.h"


ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->TargetArmLength=400;
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetRelativeLocation(FVector(0, 0, 0));
	CameraComponent->SetupAttachment(SpringArm);
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraPawn::AddSpringArmLength(float Length) const
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + Length, 100.f, 1200.f);
}
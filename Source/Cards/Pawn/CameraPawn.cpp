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

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(Mesh);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->TargetArmLength=400;
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->SetupAttachment(Mesh);
	
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
	SpringArm->TargetArmLength+=Length;
}

void ACameraPawn::AddSpringArmYawRotation(float Yaw) const
{
	const FRotator SpringArmRotation = SpringArm->GetRelativeRotation();
	FRotator SetRotation = FRotator(SpringArmRotation.Pitch,SpringArmRotation.Yaw + Yaw,SpringArmRotation.Roll);
	SpringArm->SetRelativeRotation(SetRotation);
}


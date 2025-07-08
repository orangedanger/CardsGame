// Future do it

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class CARDS_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere,Category="Components")
	USpringArmComponent* SpringArm;
public:

	void AddSpringArmLength(float Length) const;
};

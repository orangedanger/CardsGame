#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class CARDS_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere,Category="Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere,Category="Components")
	USphereComponent* SphereComponent;
};

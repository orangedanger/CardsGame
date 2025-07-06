// Future do it

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CardsPlayerController.generated.h"


struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class CARDS_API ACardsPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	
private:
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputMappingContext> Context;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> ZoomAction;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> RotateAction;

	public:
	virtual void Move( const FInputActionValue& Input);
	virtual void Zoom( const FInputActionValue& Input);
	virtual void Rotate( const FInputActionValue& Input);
};

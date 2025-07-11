// Future do it

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DebugHUD.generated.h"

class UCardsWidgetController;
/**
 * 
 */
UCLASS()
class CARDS_API ADebugHUD : public AHUD
{
	GENERATED_BODY()

	public:
	void initOverlap();

	UCardsWidgetController* GetCardsWidgetController();

	private:
	UPROPERTY()
	TObjectPtr<UCardsWidgetController> CardsWidgetController;
	UPROPERTY(EditDefaultsOnly,Category = "Widget Controller Class")
	TSubclassOf<UCardsWidgetController> CardsWidgetControllerClass;
	
};

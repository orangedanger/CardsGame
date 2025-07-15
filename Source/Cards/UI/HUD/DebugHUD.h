// Future do it

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DebugHUD.generated.h"

class UCardsWidgetController;
class UDebugWidgetController;
class UCardUserWidget;
/**
 * 
 */
UCLASS()
class CARDS_API ADebugHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void initOverlap();

	UCardsWidgetController* GetCardsWidgetController();
	UDebugWidgetController* GetDebugWidgetController();
	private:
	UPROPERTY()
	TObjectPtr<UCardUserWidget> CardsUserWidget;
	UPROPERTY(EditDefaultsOnly,Category = "Widget Class")
	TSubclassOf<UCardUserWidget> CardsUserWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UDebugWidgetController> DebugWidgetController;
	UPROPERTY(EditDefaultsOnly,Category = "Widget Controller Class")
	TSubclassOf<UDebugWidgetController> DebugWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UCardsWidgetController> CardsWidgetController;
	UPROPERTY(EditDefaultsOnly,Category = "Widget Controller Class")
	TSubclassOf<UCardsWidgetController> CardsWidgetControllerClass;
	
};

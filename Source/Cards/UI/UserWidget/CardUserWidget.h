// Future do it

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CARDS_API UCardUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);

protected:
	UPROPERTY(BlueprintReadOnly,Category="Widget Controller")
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
	
};

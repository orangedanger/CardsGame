// Future do it

#pragma once

#include "CoreMinimal.h"
#include "CardsWidgetController.h"
#include "DebugWidgetController.generated.h"

class AGridsActor;
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class CARDS_API UDebugWidgetController : public UCardsWidgetController
{
	GENERATED_BODY()
public:
	virtual void InitBroadcast() override;
	virtual void BroadcastCallbackToDependences() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta = (AllowPrivateAccess = true), Category = "DebugWidgetController")
	TSubclassOf<AGridsActor> GridActorClass;
};

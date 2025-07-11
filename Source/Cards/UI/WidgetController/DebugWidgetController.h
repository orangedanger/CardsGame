// Future do it

#pragma once

#include "CoreMinimal.h"
#include "CardsWidgetController.h"
#include "DebugWidgetController.generated.h"

class AGridsActor;
/**
 * 
 */
UCLASS()
class CARDS_API UDebugWidgetController : public UCardsWidgetController
{
	GENERATED_BODY()
public:
	virtual void InitBroadcast() override;
	virtual void BroadcastCallbackToDependences() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta = (AllowPrivateAccess = true), Category = "DebugWidgetController")
	TObjectPtr<AGridsActor> GridActor;
};

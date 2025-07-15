// Future do it

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CardsWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CARDS_API UCardsWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	//某些委托数据进行初始化播报
	virtual void InitBroadcast();

	//用于绑定委托到具体功能函数
	virtual void BroadcastCallbackToDependences();
	
};

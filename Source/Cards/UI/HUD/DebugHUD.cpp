// Future do it


#include "DebugHUD.h"
#include "../WidgetController/CardsWidgetController.h"

void ADebugHUD::initOverlap()
{
	checkf(CardsWidgetControllerClass, TEXT("Set CardsWidgetControllerClass In BP_DebugHUD"))
}

UCardsWidgetController* ADebugHUD::GetCardsWidgetController()
{
	if (CardsWidgetController == nullptr)
	{
		CardsWidgetController = NewObject<UCardsWidgetController>(this,CardsWidgetControllerClass);
		return CardsWidgetController;
	}
	return CardsWidgetController;
}
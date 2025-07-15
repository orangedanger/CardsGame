// Future do it


#include "DebugHUD.h"
#include "../WidgetController/CardsWidgetController.h"
#include "../WidgetController/DebugWidgetController.h"
#include "../UserWidget/CardUserWidget.h"

void ADebugHUD::initOverlap()
{

	checkf(CardsUserWidgetClass, TEXT("Set CardsUserWidgetClass In BP_DebugHUD"));
	checkf(DebugWidgetControllerClass, TEXT("Set DebugWidgetControllerClass In BP_DebugHUD"));
	

	UWidget* Widget = NewObject<UWidget>(this,CardsUserWidgetClass);
	CardsUserWidget = Cast<UCardUserWidget>(Widget);


	DebugWidgetController = GetDebugWidgetController();
	CardsUserWidget->SetWidgetController(DebugWidgetController);

	CardsUserWidget->AddToViewport();
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

UDebugWidgetController* ADebugHUD::GetDebugWidgetController()
{
	if (DebugWidgetController == nullptr)
	{
		DebugWidgetController = NewObject<UDebugWidgetController>(this,DebugWidgetControllerClass);
		return DebugWidgetController;
	}
	return DebugWidgetController;
}

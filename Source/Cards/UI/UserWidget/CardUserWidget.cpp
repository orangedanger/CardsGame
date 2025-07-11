// Future do it


#include "CardUserWidget.h"

void UCardUserWidget::SetWidgetController(UObject* Controller)
{
	WidgetController = Controller;
	WidgetControllerSet();
}

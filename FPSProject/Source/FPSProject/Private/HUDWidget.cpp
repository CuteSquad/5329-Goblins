// Copyright © 2018, AJACKS

#include "HUDWidget.h"
#include "WidgetTree.h"


void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DeadTextBlock = Cast<UTextBlock>(WidgetTree->FindWidget(FName("DeadText")));
}


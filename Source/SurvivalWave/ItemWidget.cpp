// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "ItemWidget.h"


UItemWidget::UItemWidget(const FObjectInitializer& init):Super(init) {
			
}

void UItemWidget::NativeConstruct() {
	Super::NativeConstruct();
}

//Tentativa de fazer aparecer na hierarchy
/*TSharedRef<SWidget> UItemWidget::RebuildWidget() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, "Rebuild!");
	
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	if (RootWidget == nullptr) {
		RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootWidget"));

		UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(RootWidget->Slot);
		if (RootWidgetSlot != nullptr) {
			RootWidgetSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
			RootWidgetSlot->SetOffsets(FMargin(0.f,0.f));
		}
		WidgetTree->RootWidget = RootWidget;
	}

	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	
	if (RootWidget != nullptr && WidgetTree != nullptr) {
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("ButtonBox"));
		RootWidget->AddChild(SizeBox);
		UCanvasPanelSlot* BoxSlot = Cast<UCanvasPanelSlot>(SizeBox->Slot);
		if (BoxSlot != nullptr) {
			BoxSlot->SetAutoSize(true);
		}
	}

	return Widget;
}*/

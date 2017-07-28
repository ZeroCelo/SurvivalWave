// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UItemWidget(const FObjectInitializer& init);

	//virtual void PreConstruct(bool IsDesignTime) override;

	//virtual bool Initialize() override;

	//TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	USizeBox* SizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	UOverlay* OverlayPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	UTextBlock* ItemText;

};

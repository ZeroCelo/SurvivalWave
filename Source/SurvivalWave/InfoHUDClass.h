// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "InfoHUDClass.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API UInfoHUDClass : public UUserWidget
{
	GENERATED_BODY()

public:

	UInfoHUDClass(const FObjectInitializer& init);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FText InfoMsg;

	UFUNCTION(BlueprintCallable)
	void SetInfo(FString Info);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InfoUpdate();
	
};

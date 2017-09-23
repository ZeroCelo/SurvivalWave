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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText ErrorMsg;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText BossMsg;

	UFUNCTION(BlueprintCallable)
	void SetInfo(FString Info);

	UFUNCTION(BlueprintCallable)
	void SetError(FString Info);

	UFUNCTION(BlueprintCallable)
	void SetBoss(FString Info);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InfoUpdate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ErrorUpdate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BossUpdate();
	
};

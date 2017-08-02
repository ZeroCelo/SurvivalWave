// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemPickup.h"
#include "ItemHUDClass.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API UItemHUDClass : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	//Careful with the default value, it must be the last key to setup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<EItemType, FText> ItemMapText;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemPickup.h"

#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInventoryWidget(const FObjectInitializer& init);

	//virtual void PreConstruct(bool IsDesignTime) override;

	//virtual bool Initialize() override;

	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 GetWeaponID(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 GetItemID(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Data")
	FItem GetItem(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetItem(int32 index, FItem new_value);

	UFUNCTION(BlueprintCallable, Category = "Data")
	FItem DropItem(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Data")
	FItem DropWeapon(int32 index);

	//Try to add item to inventory, return quantity consumed if any
	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 AddItem(FItem new_item);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void MoveItem(int32 source, int32 destiny);

	UFUNCTION(BlueprintImplementableEvent, Category = "Data")
	void RefreshItem(int32 index);

	UFUNCTION(BlueprintImplementableEvent, Category = "Data")
	void RefreshWeapon(int32 index);
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 items_index_free;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<class UUserWidget> ButtonClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<UUserWidget*> buttons;
	//TArray<UButton*> buttons;

	//The Uniform Grid Panel for the items buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	UUniformGridPanel* grid_panel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FItem> items;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<UUserWidget*> WeaponButtons;

	//The Uniform Grid Panel for the weapon buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	UUniformGridPanel* WeaponPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FItem> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FMargin GridPad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float items_width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float items_height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 WeaponsNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 items_w;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 items_h;

};

// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "InventoryWidget.h"

#include "Engine.h"


/*#if UE_BUILD_DEBUG
#define Print(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT(x));}
#endif*/

UInventoryWidget::UInventoryWidget(const FObjectInitializer& init):Super(init) {
	//if(grid_panel == nullptr)
	
	//GridPad = CreateDefaultSubObject<FMargin>(TEXT("GridPads"));
	//FMargin mar();
	
	items_w = 3;
	items_h = 3;
	items_width = 180.0;
	items_height = 90.0;
	WeaponsNum = 2;
	PlayerIndex = 0;
}

/*bool UInventoryWidget::Initialize() {
	bool ret = Super::Initialize();
	//grid_panel = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass(), TEXT("InventoryGrid"));
	return ret;
}*/

/*bool UInventoryWidget::Initialize() {
	bool ret = Super::Initialize();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, "Init!!!");
	//UPanelWidget* Root = Cast<UPanelWidget>(GetRootWidget());
	//if (grid_panel == nullptr && WidgetTree != nullptr && Root != nullptr) {
		//grid_panel = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass(), TEXT("InventoryGrid"));
		//grid_panel->SetMinDesiredSlotWidth(items_width);
		//grid_panel->SetMinDesiredSlotHeight(items_height);
		//Root->AddChild(grid_panel);
	//}
	return ret;
}*/

TSharedRef<SWidget> UInventoryWidget::RebuildWidget() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, "Rebuild!");
	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	/*
	if (grid_panel == nullptr) {
		grid_panel = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass(), TEXT("InventoryGrid"));
		grid_panel->SetMinDesiredSlotWidth(items_width);
		grid_panel->SetMinDesiredSlotHeight(items_height);
	}*/

	return Widget;
}

void UInventoryWidget::NativeConstruct() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, "NativeConstruct");
	int32 tot = items_w*items_h;
	
	if (grid_panel == nullptr) {
		grid_panel = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass(), TEXT("InventoryGrid"));
		grid_panel->SetMinDesiredSlotWidth(items_width);
		grid_panel->SetMinDesiredSlotHeight(items_height);
		grid_panel->SetSlotPadding(GridPad);
	}

	bool bitems_change = false;
	if (items.Num() != tot) {
		items.Empty();
		items.SetNum(tot);
		bitems_change = true;
		
		/*if (items.Num() > 1 && items.Num() < 10) {
			items[1].id = 10; items[1].quantity = 11;
			items[8].id = 10; items[8].quantity = 21;
			//items[2].id = 10; items[2].type = EItemType::IT_LifeS;
			//items[4].id = 10; items[4].type = EItemType::IT_LifeM;
			//items[6].id = 10; items[6].type = EItemType::IT_LifeL;
		}*/
	}
		
	if (bitems_change) {
		buttons.Empty();
		//for (int32 i = 0; i < tot; i++) {
		for (int32 y = 0; y < items_h; y++) {
			for (int32 x = 0; x < items_w; x++) {
				int32 i = y*items_w + x;
				if (ButtonClass != nullptr) {
					FString button_name("Button");
					button_name.AppendInt(i);
					//buttons.Add(WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("MyButtonName")));
					//buttons.Add(WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), *button_name));
					//}
					APlayerController* PlayController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
					buttons.Add(CreateWidget<UUserWidget>(PlayController, ButtonClass));
					//UButton* but = Cast<UButton>(buttons[i]);
					//if(but != nullptr)
					//but->OnClicked.AddDynamic(this, &UInventoryWidget::ClickItem);

					grid_panel->AddChild(buttons[i]);
					if (UUniformGridSlot* slot = Cast<UUniformGridSlot>(buttons[i]->Slot)) {
						slot->SetColumn(x);
						slot->SetRow(y);
					}
				}
			}
			
		}
	}

	//Weapon Inventory setup
	if (WeaponPanel == nullptr) {
		WeaponPanel = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass(), TEXT("WeaponGrid"));
		WeaponPanel->SetMinDesiredSlotWidth(items_width);
		WeaponPanel->SetMinDesiredSlotHeight(items_height);
		WeaponPanel->SetSlotPadding(GridPad);
	}

	bool bweapon_change = false;
	if (Weapons.Num() != WeaponsNum) {
		Weapons.Empty();
		Weapons.SetNum(WeaponsNum);
		bweapon_change = true;
	}

	if (bweapon_change) {
		WeaponButtons.Empty();
		for (int32 i = 0; i < Weapons.Num(); i++) {
			if (ButtonClass != nullptr) {
				FString button_name("ButtonW");
				button_name.AppendInt(i);
				//buttons.Add(WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("MyButtonName")));
				//buttons.Add(WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), *button_name));
				//}
				APlayerController* PlayController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				WeaponButtons.Add(CreateWidget<UUserWidget>(PlayController, ButtonClass));
				//UButton* but = Cast<UButton>(buttons[i]);
				//if(but != nullptr)
				//but->OnClicked.AddDynamic(this, &UInventoryWidget::ClickItem);
				WeaponPanel->AddChild(WeaponButtons[i]);
				if (UUniformGridSlot* slot = Cast<UUniformGridSlot>(WeaponButtons[i]->Slot)) {
					//slot->SetColumn(x);
					slot->SetRow(i);
				}
			}
		}
	}

	Super::NativeConstruct();	//Construct of UMG
/*#if UE_BUILD_DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vel %f,%f,%f"), GetVelocity().X, GetVelocity().Y, GetVelocity().Z));

	Print("NativeConstruct");
#endif*/
	
}

int32 UInventoryWidget::GetWeaponID(int32 index) {
	return Weapons[index].id;
}

int32 UInventoryWidget::GetItemID(int32 index) {
	return items[index].id;
}

FItem UInventoryWidget::GetItem(int32 index) {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("GetItem %d, id:%d quant:%d Type:%s"), index, items[index].id, items[index].quantity, *FItem::GetItemEnumAsString(items[index].type)));
	return items[index];
}

void UInventoryWidget::SetItem(int32 index, FItem new_value) {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("SetItem %d, id:%d quant:%d Type:%s"),index,new_value.id,new_value.quantity, *FItem::GetItemEnumAsString(new_value.type)));
	items[index] = new_value;
	if (items[index].id < 0)
		items[index].id = new_value.id;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("NewItem %d, id:%d quant:%d Type:%s"), index, items[index].id, items[index].quantity, *FItem::GetItemEnumAsString(items[index].type)));
}

void UInventoryWidget::ConsumeItem(EItemType it, int32 consume_need) {
	if (ItemMapTotal.Contains(it)) {
		ItemMapTotal[it] -= consume_need;
	}
	if (ItemMapList.Contains(it)) {
		int32 remove_count = 0;
		int32 need_remaining = consume_need;
		for (int32 i = ItemMapList[it].Num() - 1; i >= 0; i--) {
			int32 index = ItemMapList[it][i];
			int32 temp = need_remaining - items[index].quantity;
			if (temp >= 0) {
				remove_count++;
				need_remaining = temp;
			}
			else {
				items[index].quantity -= need_remaining;
				break;
			}
		}
		for (int32 i = 0; i < remove_count; i++) {
			int32 index = ItemMapList[it].Pop();
			items[index].id = -1;
			RefreshItem(index);
		}
	}
}

void UInventoryWidget::ConsumeItemAt(int32 index, int32 consume_need) {
	if (items[index].quantity > consume_need) {
		items[index].quantity -= consume_need;
	}
	else {
		items[index].id = -1;
	}
	if (ItemMapTotal.Contains(items[index].type))
		ItemMapTotal[items[index].type] -= items[index].quantity;
	if (ItemMapList.Contains(items[index].type)) {
		int32 foundIndex = ItemMapList[items[index].type].Find(index);
		if (foundIndex != INDEX_NONE)ItemMapList[items[index].type].RemoveAt(foundIndex);
	}
	RefreshItem(index);
}

FItem UInventoryWidget::DropItem(int32 index) {
	FItem drop = items[index]; drop.id = items[index].id;
	items[index].id = -1;
	if (ItemMapTotal.Contains(drop.type))
		ItemMapTotal[drop.type] -= drop.quantity;
	if (ItemMapList.Contains(drop.type)) {
		int32 foundIndex = ItemMapList[drop.type].Find(index);
		if (foundIndex != INDEX_NONE)ItemMapList[drop.type].RemoveAt(foundIndex);
	}
	RefreshItem(index);
	return drop;
}

FItem UInventoryWidget::DropWeapon(int32 index) {
	FItem drop = Weapons[index]; drop.id = Weapons[index].id;
	Weapons[index].id = -1;
	RefreshWeapon(index);
	return drop;
}

int32 UInventoryWidget::GetItemSum(EItemType it) {
	if (ItemMapTotal.Contains(it))return ItemMapTotal[it];
	else return 0;
}

void UInventoryWidget::MoveItemList(int32 source, int32 destiny) {
	FItem sourceItem = items[source]; sourceItem.id = items[source].id;
	FItem destinyItem = items[destiny]; destinyItem.id = items[destiny].id;
	
	/* Possible Cases of moving items of type X and Y , {'Empty'}, '->' = move operation
		Src	Dst		Src	Dst
	1	X	{}	->	{}	X
	2	X	X	->	X	X
	3	X	X	->	{}	X
	4	X	Y	->	Y	X
	*/
	
	if (ItemMapList.Contains(destinyItem.type)) {	//Case 1 and 4 for destiny, no need for 2 and 3
		if (ItemMapList[destinyItem.type].Find(destiny) == INDEX_NONE)
			ItemMapList[destinyItem.type].Add(destiny);
	}
	
	if (sourceItem.id < 0) {	//Case 1 and 3 for source
		if (ItemMapList.Contains(sourceItem.type)) {
			int32 foundIndex = ItemMapList[sourceItem.type].Find(source);
			if (foundIndex != INDEX_NONE)
				ItemMapList[sourceItem.type].RemoveAt(foundIndex);
		}
	}
	else {	//Case 2 and 4 for source
		if (sourceItem.type != destinyItem.type) {	//Case 4
			int32 foundIndexSrc = ItemMapList[sourceItem.type].Find(destiny);
			int32 foundIndexDst = ItemMapList[destinyItem.type].Find(source);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Src %d Dst %d"), foundIndexSrc,foundIndexDst));
			if (foundIndexSrc != INDEX_NONE)	
				ItemMapList[sourceItem.type].RemoveAt(foundIndexSrc);
			if (foundIndexDst != INDEX_NONE)
				ItemMapList[destinyItem.type].RemoveAt(foundIndexDst);
			ItemMapList[sourceItem.type].Add(source);
		}
	}
	/*for (auto& Elem : ItemMapList) {
		FString strType = FItem::GetItemEnumAsString(Elem.Key);
		FString strOut("List<"); strOut += strType;	strOut += "> ";
		for (int32 i = 0; i < ItemMapList[Elem.Key].Num(); i++) {
			strOut.AppendInt(ItemMapList[Elem.Key][i]); strOut += ' ';
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::Printf(TEXT("%s"), *strOut));
	}*/
}

void UInventoryWidget::AddItemList(FItem new_item, int32 new_quantity, int32 index) {
	if (ItemMapTotal.Contains(new_item.type))
		ItemMapTotal[new_item.type] += new_quantity;
	else
		ItemMapTotal.Add(new_item.type,new_quantity);

	if (ItemMapList.Contains(new_item.type)) {
		int32 foundIndex = -1;
		if (!ItemMapList[new_item.type].Find(index, foundIndex)) {
			ItemMapList[new_item.type].Add(index);
		}
	}
	else {
		ItemMapList.Add(new_item.type);
		ItemMapList[new_item.type].Add(index);
	}

	/*for (auto& Elem : ItemMapList)	{
		FString strType = FItem::GetItemEnumAsString(Elem.Key);
		FString strOut("List<"); strOut += strType;	strOut += "> ";
		for (int32 i = 0; i < ItemMapList[Elem.Key].Num(); i++) {
			strOut.AppendInt(ItemMapList[Elem.Key][i]); strOut += ' ';
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%s"), *strOut));
	}*/
}

int32 UInventoryWidget::AddItem(FItem new_item) {
	FItem temp = new_item; temp.id = new_item.id;
	FString str = FItem::GetItemEnumAsString(new_item.type);
	if (str.Contains("Gun")) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, "AddWeapon");
		for (int32 i = 0; i < Weapons.Num(); i++) {
			if (Weapons[i].id < 0) {
				Weapons[i] = temp; Weapons[i].id = temp.id;
				RefreshWeapon(i); return -1;
			}
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, "AddItem");
		for (int32 i = 0; i < items.Num(); i++) {
			if (items[i].id < 0) {
				items[i] = temp; items[i].id = temp.id;
				RefreshItem(i);	
				//if (ItemMapTotal.Contains(temp.type))ItemMapTotal[temp.type] += temp.quantity;
				//else ItemMapTotal.Add(temp.type, temp.quantity);
				AddItemList(temp,temp.quantity,i);
				return -1;
			}
			else if (items[i].type == new_item.type) {
				int32 sum = items[i].quantity + temp.quantity;
				if (sum <= items[i].limit) {
					items[i].quantity = sum;
					RefreshItem(i);	
					//if (ItemMapTotal.Contains(temp.type))ItemMapTotal[temp.type] += temp.quantity;
					//else ItemMapTotal.Add(temp.type, temp.quantity);
					AddItemList(temp, temp.quantity, i);
					return -1;
				}
				else {
					//if (ItemMapTotal.Contains(temp.type))ItemMapTotal[temp.type] += items[i].limit - items[i].quantity;
					//else ItemMapTotal.Add(temp.type, items[i].limit - items[i].quantity);
					AddItemList(temp, items[i].limit - items[i].quantity, i);
					int32 diff = sum - items[i].limit;
					items[i].quantity = items[i].limit;
					temp.quantity = diff;
				}
			}
		}
	}
	return temp.quantity;
	//items[index] = new_item;
}

void UInventoryWidget::MoveItem(int32 source, int32 destiny) {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, "MoveItem");
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("MoveItem %d, %d"), source, destiny));
	if (items[destiny].id != -1) {
		if (items[destiny].type == items[source].type) {
			int32 sum = items[destiny].quantity + items[source].quantity;
			if (sum <= items[destiny].limit) {
				items[destiny].quantity = sum;
				items[source].id = -1;
			}
			else {
				if (items[destiny].quantity == items[destiny].limit){
					int32 diff = sum - items[destiny].limit;
					items[source].quantity = items[destiny].limit;
					items[destiny].quantity = diff;
				}
				else {
					int32 diff = sum - items[destiny].limit;
					items[destiny].quantity = items[destiny].limit;
					items[source].quantity = diff;
				}
			}
		}
		else {
			FItem temp = items[source]; //temp.id = items[source].id;
			items[source] = items[destiny];
			items[destiny] = temp;
		}
			//items[destiny] = items[source];
	}
	else {
		items[destiny] = items[source]; items[destiny].id = items[source].id;
		items[source].id = -1;
	}
	MoveItemList(source,destiny);
	RefreshItem(source);
	RefreshItem(destiny);
}

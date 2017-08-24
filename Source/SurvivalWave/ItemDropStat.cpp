// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "ItemDropStat.h"

#include "EngineGlobals.h"

// Sets default values for this component's properties
UItemDropStat::UItemDropStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	NothingChance = 30;
	MaxChance = 100;
}


// Called when the game starts
void UItemDropStat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Item %d, Quant %d"), ItemsDrop.Num(), ItemsQuantity.Num()));
	ItemsDrop.Sort();
	ItemsQuantity.Sort();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Item %d, Quant %d"), ItemsDrop.Num(),ItemsQuantity.Num()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("ItemSimple %d "), ItemsSimple.Num()));
}


// Called every frame
void UItemDropStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UItemDropStat::GetDropQuantity() {
	int32 Max = 0;
	TArray<int32> Quant;
	int32 resul = 1;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Item Quant %d"), ItemsQuantity.Num()));
	for (int32 i = 0; i < ItemsQuantity.Num(); i++) {
		Quant.Add(ItemsQuantity[i].ChancePoints + Max);
		Max += ItemsQuantity[i].ChancePoints;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Quant Max=%d Chance=%d"),Max, ItemsQuantity[i].ChancePoints));
	}
	if (Max > 0) {
		int32 ResultChance = FMath::RandRange(1, Max);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Quant Result=%d"), ResultChance));
		for (int32 i = 0; i < Quant.Num(); i++) {
			if (ResultChance <= Quant[i]) {
				resul = ItemsQuantity[i].QuantityMax;
				break;
			}
		}
	}
	return resul;
}

FItem UItemDropStat::GetDropItem() {
	int32 Max = 0;
	FItem drop;
	TArray<int32> Items;
	for (int32 i = 0; i < ItemsDrop.Num(); i++) {
		Items.Add(ItemsDrop[i].ChancePoints + Max);
		Max += ItemsDrop[i].ChancePoints;
	}
	if (Max > 0) {
		int32 ResultChance = FMath::RandRange(1, Max);
		for (int32 i = 0; i < Items.Num(); i++) {
			if (ResultChance <= Items[i]) {
				int32 ResultQuantity = FMath::RandRange(ItemsDrop[i].QuantityMin, ItemsDrop[i].QuantityMax);
				drop.quantity = ResultQuantity;
				drop.type = ItemsDrop[i].ItemType;
				break;
			}
		}
	}
	
	return drop;
}

bool UItemDropStat::GetDrop() {
	//return true;
	//int32 SomethingChance = MaxChance - NothingChance;
	int32 ResultChance = FMath::RandRange(1, MaxChance);
	if (ResultChance > NothingChance) 
		return true;
	return false;
}

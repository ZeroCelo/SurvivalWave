// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "KeyItemInventory.h"


// Sets default values for this component's properties
UKeyItemInventory::UKeyItemInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKeyItemInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKeyItemInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UKeyItemInventory::HasKey(float KeyItem) {
	return Inventory.Contains(KeyItem);
}

void UKeyItemInventory::AddKey(float KeyItem){
	if (!Inventory.Contains(KeyItem))
		Inventory.Add(KeyItem);
}

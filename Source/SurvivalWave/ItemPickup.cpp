// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "ItemPickup.h"

uint32 FItem::id_vault = 0;
TMap<EItemType, UMaterialInstance*> AItemPickup::ItemMapMaterial;
TMap<EItemType, UStaticMesh*> AItemPickup::ItemMapMesh;
TMap<EItemType, FVector> AItemPickup::ItemMapSize;
TMap<EItemType, int32> AItemPickup::ItemMapLimit;

AItemPickup::AItemPickup(){
	GetMesh()->SetSimulatePhysics(true);

	
}

void AItemPickup::BeginPlay() {
	ItemStat.GenerateID();
}

void AItemPickup::OnConstruction(const FTransform& Transform) {

}

void AItemPickup::CheckLimit_Implementation() {
	if (ItemMapLimit.Contains(ItemStat.type)) {
		ItemStat.limit = ItemMapLimit[ItemStat.type];
		if (ItemStat.quantity > ItemStat.limit)
			ItemStat.quantity = ItemStat.limit;
	}
	else {
		ItemStat.quantity = 1;
		ItemStat.limit = 1;
	}
}

void AItemPickup::WasCollected_Implementation() {
	//Use the base pickup behavior
	Super::WasCollected_Implementation();

	//Destroy the battery
	Destroy();
}

int32 AItemPickup::ItemMapGetLimit(EItemType type) {
	return ItemMapLimit[type];
}

UMaterialInstance* AItemPickup::ItemMapGetMaterial(EItemType type) {
	return ItemMapMaterial[type];
}

UStaticMesh* AItemPickup::ItemMapGetMesh(EItemType type) {
	return ItemMapMesh[type];
}

FVector AItemPickup::ItemMapGetSize(EItemType type) {
	return ItemMapSize[type];
}

bool AItemPickup::ItemMapContainsLimit(EItemType type) {
	return ItemMapLimit.Contains(type);
}

bool AItemPickup::ItemMapContainsMaterial(EItemType type) {
	return ItemMapMaterial.Contains(type);
}

bool AItemPickup::ItemMapContainsMesh(EItemType type) {
	return ItemMapMesh.Contains(type);
}

bool AItemPickup::ItemMapContainsSize(EItemType type) {
	return ItemMapSize.Contains(type);
}

void AItemPickup::ItemMapAdd(EItemType type, UMaterialInstance* Mat, UStaticMesh* Mesh, FVector Size) {
	ItemMapAddMaterial(type, Mat);
	ItemMapAddMesh(type, Mesh);
	ItemMapAddSize(type, Size);
}

void AItemPickup::ItemMapAddLimit(EItemType type, int32 Limit) {
	if (!ItemMapLimit.Contains(type)) {
		ItemMapLimit.Add(type, Limit);
	}
}

void AItemPickup::ItemMapAddMaterial(EItemType type, UMaterialInstance* Mat) {
	if (!ItemMapMaterial.Contains(type)) {
		ItemMapMaterial.Add(type, Mat);
	}
}

void AItemPickup::ItemMapAddMesh(EItemType type, UStaticMesh* Mesh) {
	if (!ItemMapMesh.Contains(type)) {
		ItemMapMesh.Add(type, Mesh);
	}
}

void AItemPickup::ItemMapAddSize(EItemType type, FVector Size) {
	if (!ItemMapSize.Contains(type)) {
		ItemMapSize.Add(type, Size);
	}
}

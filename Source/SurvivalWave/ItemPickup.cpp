// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "ItemPickup.h"

uint32 FItem::id_vault = 0;

AItemPickup::AItemPickup(){
	GetMesh()->SetSimulatePhysics(true);

	
}

void AItemPickup::BeginPlay() {
	ItemStat.GenerateID();
}

void AItemPickup::WasCollected_Implementation() {
	//Use the base pickup behavior
	Super::WasCollected_Implementation();

	//Destroy the battery
	Destroy();
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "ItemSpawner.h"
#include "ItemPickup.h"


AItemSpawner::AItemSpawner() {
	DropStats = CreateDefaultSubobject<UItemDropStat>(TEXT("DropComponent"));
	this->AddOwnedComponent(DropStats);
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemSpawner::PostSpawnActor(AActor* Act) {
	AItemPickup* Pickup = Cast<AItemPickup>(Act);
	if (Pickup != nullptr) {
		FItem ite = DropStats->GetDropItem();
		Pickup->ItemStat = ite;
		Pickup->CheckItem();
	}
}

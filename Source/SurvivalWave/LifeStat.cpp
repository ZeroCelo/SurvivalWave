// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "LifeStat.h"


// Sets default values for this component's properties
ULifeStat::ULifeStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	Life = 100.0f;
	LifeMax = 100.0f;
}


// Called when the game starts
void ULifeStat::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void ULifeStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool ULifeStat::IsDead() {
	return (Life <= 0.0f);
}

void ULifeStat::RestoreLife_Implementation(float rest) {
	Life += rest;
	if (Life >= LifeMax)Life = LifeMax;
}

void ULifeStat::TakeDamage_Implementation(float dmg) {
	Life -= dmg;
	if (Life <= 0.0f)Life = 0.0f;
}

void ULifeStat::TakeDamageMultiplier_Implementation(float dmg,float multi) {
	Life -= (dmg*multi);
	if (Life <= 0.0f)Life = 0.0f;
}

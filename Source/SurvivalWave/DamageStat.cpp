// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "DamageStat.h"


// Sets default values for this component's properties
UDamageStat::UDamageStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	Damage = 1.0f;
	DamageRange = 5000.0f;
	DamageRate = 0.25f;
}


// Called when the game starts
void UDamageStat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UDamageStat::GetDamage_Implementation() {
	return Damage;
}

float UDamageStat::GetDamageRange_Implementation() {
	return DamageRange;
}

float UDamageStat::GetDamageRate_Implementation() {
	return DamageRate;
}

void UDamageStat::CopyDamage_Implementation(const UDamageStat* dam) {
	if (dam != nullptr) {
		Damage = dam->Damage;
		DamageRange = dam->DamageRange;
		DamageRate = dam->DamageRate;
	}
}

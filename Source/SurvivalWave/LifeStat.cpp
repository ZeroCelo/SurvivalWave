// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "LifeStat.h"
//#include "DamageStat.h"

#include "EngineGlobals.h"
//#include "Engine.h"

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

bool ULifeStat::NeedHealing() {
	return (Life < LifeMax);
}

bool ULifeStat::IsDead() {
	return (Life <= 0.0f);
}

void ULifeStat::RestoreLife_Implementation(float rest) {
	Life += rest;
	if (Life >= LifeMax)Life = LifeMax;
}

void ULifeStat::TakeDamage_Implementation(UDamageStat* dmg) {
	if (dmg != nullptr) {
		Life -= dmg->GetDamage();
		if (Life <= 0.0f)Life = 0.0f;
	}
}

void ULifeStat::TakeDamageMultiplier_Implementation(float dmg,float multi) {
	Life -= (dmg*multi);
	if (Life <= 0.0f)Life = 0.0f;
}

void ULifeStat::DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LifeStat Detect")));
	//UDamageStat* Dam = OtherActor->FindComponentByClass<UDamageStat>();
	UDamageStat* Dam = OtherActor->FindComponentByClass<UDamageStat>();

	if (Dam != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Damage!!! %f"), Dam->GetDamage()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Damage!!!")));
		TakeDamage(Dam);
	}
}

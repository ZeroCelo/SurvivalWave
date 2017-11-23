// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "LifeStat.h"
//#include "DamageStat.h"
//#include "Projectile.h"

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
	Shield = 50.0f;
	ShieldMax = 50.0f;
	ShieldRechargeAmount = 1.0f;
	ShieldRechargeRate = 1.0f;
	ShieldRechargeWait = 10.0f;
	bShieldActive = false;
	bRecharging = false;

}


// Called when the game starts
void ULifeStat::BeginPlay()
{
	Super::BeginPlay();
	ShieldRechargeWait = ShieldRechargeWait;
	ShieldRechargeRate = ShieldRechargeRate;
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

bool ULifeStat::IsShieldRecharging() {
	return bRecharging;
}

bool ULifeStat::IsShieldActive() {
	return bShieldActive && Shield > 0.0f;
}

void ULifeStat::SetShieldActive(bool bActive) {
	bShieldActive = bActive;
	if (bShieldActive)
		ShieldCallRecharge(false);
}

void ULifeStat::ShieldCallRecharge(bool bWait) {
	if (bWait) {
		GetWorld()->GetTimerManager().ClearTimer(RechargeTimer);
		GetWorld()->GetTimerManager().SetTimer(RechargeTimer, this, &ULifeStat::ShieldRecharge, ShieldRechargeWait, false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LifeStat Shield Wait %f"),ShieldRechargeWait));
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(RechargeTimer, this, &ULifeStat::ShieldRecharge, ShieldRechargeRate, false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LifeStat Shield Call %f"), ShieldRechargeRate));
	}
}

void ULifeStat::ShieldRecharge() {
	if (!IsDead()) {
		bRecharging = true;
		Shield += ShieldRechargeAmount;
		if (Shield > ShieldMax)Shield = ShieldMax;
		if (Shield <= 0.0f)Shield = 0.0f;

		if (Shield < ShieldMax && DoesShieldRecharge())
			ShieldCallRecharge();
		else
			bRecharging = false;
	}
}

void ULifeStat::RestoreLife_Implementation(float rest) {
	Life += rest;
	if (Life >= LifeMax)Life = LifeMax;
	if (Life <= 0.0f)Life = 0.0f;
}

void ULifeStat::TakeDamage_Implementation(UDamageStat* dmg) {
	if (dmg != nullptr) {
		if (DoesShieldRecharge()) {
			bRecharging = false;
			if (Shield > 0.0f) {
				Shield -= dmg->GetDamage();
				if (Shield > ShieldMax)Shield = ShieldMax;
				if (Shield <= 0.0f)Shield = 0.0f;
			}
			else {
				Life -= dmg->GetDamage();
				if (Life <= 0.0f)Life = 0.0f;
				if (Life >= LifeMax)Life = LifeMax;
			}
			ShieldCallRecharge(true);
		}
		else {
			Life -= dmg->GetDamage();
			if (Life <= 0.0f)Life = 0.0f;
			if (Life >= LifeMax)Life = LifeMax;
		}
	}
}

void ULifeStat::TakeDamageMultiplier_Implementation(float dmg,float multi) {

	if (DoesShieldRecharge()) {
		bRecharging = false;
		if (Shield > 0.0f) {
			Shield -= (dmg*multi);
			if (Shield > ShieldMax)Shield = ShieldMax;
			if (Shield <= 0.0f)Shield = 0.0f;
		}
		else {
			Life -= (dmg*multi);
			if (Life <= 0.0f)Life = 0.0f;
			if (Life >= LifeMax)Life = LifeMax;
		}
		ShieldCallRecharge(true);
	}
	else {
		Life -= (dmg*multi);
		if (Life <= 0.0f)Life = 0.0f;
		if (Life >= LifeMax)Life = LifeMax;
	}
}

void ULifeStat::DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LifeStat Detect")));
	//UDamageStat* Dam = OtherActor->FindComponentByClass<UDamageStat>();
	UDamageStat* Dam = OtherActor->FindComponentByClass<UDamageStat>();

	if (Dam != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Damage!!! %f, %f, %f"), Dam->GetDamage(),Dam->GetDamageRange(),Dam->GetDamageRate()));
		
		TakeDamage(Dam);
	}
}

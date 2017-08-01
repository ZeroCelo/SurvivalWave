// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "Weapon_Projectile.h"

AWeapon_Projectile::AWeapon_Projectile() :Super() {
	LastFireTime = 0.0f;
}

void AWeapon_Projectile::StartFire_Implementation() {
	float game_time = GetWorld()->GetTimeSeconds();
	float dif_time = game_time - LastFireTime;
	if (dif_time >= DamageRate) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shoot")));
		if (ProjectileClass != nullptr) {
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.Instigator = Instigator;
			AProjectile* proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation(), SpawnInfo);
			if (proj != nullptr) {
				proj->MaxDistance = DamageRange;
				proj->Damage = Damage;
			}
		}
		LastFireTime = game_time;
		SimulateFire();
	}
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeapon_Projectile::StartFire, LastFireTime + DamageRate - game_time, false);
	GetWorld()->GetTimerManager().SetTimer(MuzzleTimer, this, &AWeapon_Projectile::SimulateFireStop, muzzle_time, false);
}

void AWeapon_Projectile::StopFire_Implementation() {
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	//GetWorld()->GetTimerManager().ClearTimer(MuzzleTimer);

	//SimulateFireStop();
}

void AWeapon_Projectile::SimulateFire_Implementation() {
	if (MuzzleFX != nullptr && MuzzlePSC == nullptr) {
		//MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FirePoint, NAME_None,FirePoint->RelativeLocation,FirePoint->RelativeRotation);
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FirePoint);
		//MuzzlePSC->bAutoDestroy = false;
		//MuzzlePSC->bAllowRecycling = true;
		//MuzzlePSC->bOwnerNoSee = false;
		//MuzzlePSC->bOnlyOwnerSee = true;
	}

}

void AWeapon_Projectile::SimulateFireStop_Implementation() {
	if (MuzzlePSC != nullptr){
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = nullptr;
	}
}


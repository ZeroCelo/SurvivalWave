// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "Weapon_Projectile.h"

AWeapon_Projectile::AWeapon_Projectile() :Super() {
	LastFireTime = 0.0f;
}

void AWeapon_Projectile::Shoot_Implementation() {
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shoot")));
	if (ProjectileClass != nullptr) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = Instigator;
		AProjectile* proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation(), SpawnInfo);
		if (proj != nullptr) {
			proj->DamageStats->CopyDamage(DamageStats);
		}
	}
}


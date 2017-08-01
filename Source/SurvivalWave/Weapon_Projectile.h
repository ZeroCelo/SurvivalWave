// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "Projectile.h"

#include "Weapon_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AWeapon_Projectile : public AWeapon
{
	GENERATED_BODY()

public:
	AWeapon_Projectile();

	FTimerHandle FireTimer;
	FTimerHandle MuzzleTimer;

	float LastFireTime;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

	/** FX for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* MuzzleFX;

	/** spawned component for muzzle FX */
	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;
	
	virtual void StartFire_Implementation();
	virtual void StopFire_Implementation();
	virtual void SimulateFire_Implementation();
	virtual void SimulateFireStop_Implementation();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "Weapon_Instant.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AWeapon_Instant : public AWeapon
{
	GENERATED_BODY()

	AWeapon_Instant();

	FTimerHandle FireTimer;
	FTimerHandle MuzzleTimer;

	float LastFireTime;

	/** FX for the shot impact*/
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* ImpactFX;

	/** FX for the Trace shot */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* TraceFX;

	/** FX for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleFX;

	/** spawned component for muzzle FX */
	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;

	virtual void StartFire_Implementation();
	virtual void StopFire_Implementation();
	virtual void SimulateFire_Implementation();
	virtual void SimulateFireStop_Implementation();
	
	void ShootImpact(FHitResult HitResult);
};

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

	/** FX for the shot impact*/
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* ImpactFX;

	/** FX for the Trace shot */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* TraceFX;

	virtual void Shoot_Implementation();
	
	void ShootImpact(FHitResult HitResult);
};

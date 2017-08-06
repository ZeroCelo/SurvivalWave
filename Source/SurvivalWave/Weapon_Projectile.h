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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;
	
	virtual void Shoot_Implementation();
	
};

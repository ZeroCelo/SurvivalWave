// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemPickup.h"
#include "DamageStat.h"

#include "Weapon.generated.h"

UCLASS()
class SURVIVALWAVE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UDamageStat* DamageStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class USkeletalMeshComponent* SkelMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	class UArrowComponent* FirePoint;

	FTimerHandle FireTimer;
	FTimerHandle MuzzleTimer;

	float LastFireDiff;
	float LastFireTime;
		
	/** FX for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	UParticleSystem* MuzzleFX;

	/** spawned component for muzzle FX */
	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int32 AmmoMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bAmmoInfinite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bReloadInfinite;

	//Time in seconds for the Reload Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float ReloadTime;

	//Time in seconds for the muzzle Effects to stop spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float muzzle_time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemType WeaponAmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UCameraComponent* TargetCamera;
	
	UFUNCTION(BlueprintCallable, Category = "Data")
	EItemType GetWeaponType();

	UFUNCTION(BlueprintCallable, Category = "Data")
	EItemType GetWeaponAmmoType();

	UFUNCTION(BlueprintCallable, Category = "Data")
	float ReloadAmmo(float new_ammo);

	UFUNCTION(BlueprintCallable, Category = "Data")
	bool IsAmmoEmpty();

	UFUNCTION(BlueprintCallable, Category = "Data")
	bool IsAmmoFull();

	UFUNCTION(BlueprintCallable, Category = "Data")
	bool IsReloadInfinite();

	UFUNCTION(BlueprintCallable, Category = "Data")
	bool ShouldReload();

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetWeaponAmmo();

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetWeaponAmmoMax();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fire")
	void StartFire();
	virtual void StartFire_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fire")
	void StopFire();
	virtual void StopFire_Implementation();
	
	/* Start Simulating Firing Effects*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fire")
	void SimulateFire();
	virtual void SimulateFire_Implementation();
	
	/* Stop Simulating Firing Effects*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fire")
	void SimulateFireStop();
	virtual void SimulateFireStop_Implementation();

	/* Actual Firing of bullet*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fire")
	void Shoot();
	virtual void Shoot_Implementation();
};

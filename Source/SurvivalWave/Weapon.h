// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemPickup.h"
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
	class USkeletalMeshComponent* SkelMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	class UArrowComponent* FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float AmmoMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bAmmoInfinite;

	//Time in seconds for the muzzle Effects to stop spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float muzzle_time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemType WeaponType;
		
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
};

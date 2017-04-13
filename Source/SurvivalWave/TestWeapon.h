// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"

#include "TestWeapon.generated.h"

UCLASS()
class SURVIVALWAVE_API ATestWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class USkeletalMeshComponent* SkelMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	class UArrowComponent* FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damage;

	//In Unreal units
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damage_range;

	//Firing rate in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damage_rate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float damage_ammo_cap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	float damage_ammo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire")
	FTimerHandle FireTimer;

	FTimerHandle MuzzleTimer;

	float last_fire;

	/* Debug shot trace lines*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	bool trace_debug;

	/** FX for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* MuzzleFX;

	/** spawned component for muzzle FX */
	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;

	//UFUNCTION(BlueprintImplementableEvent)
	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
	void ShootImpact(FHitResult HitResult);
	void StartFire();
	void StopFire();
	void Shoot();
	/* Start Simulating Firing Effects*/
	virtual void SimulateFire();
	/* Stop Simulating Firing Effects*/
	virtual void SimulateFireStop();
};

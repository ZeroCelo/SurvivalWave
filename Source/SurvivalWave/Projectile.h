// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "Components/ArrowComponent.h"

#include "Projectile.generated.h"


UCLASS()
class SURVIVALWAVE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDamageStat* DamageStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* FXPoint;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMove;

	/** FX for the shot impact*/
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImpactFX;

	/** FX for muzzle flash */
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ProjectileFX;

	/** spawned component for muzzle FX */
	UPROPERTY(Transient)
	UParticleSystemComponent* ProjectilePSC;
		
	FVector StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAreaDamage;

	UFUNCTION(BlueprintCallable)
	bool DoesAreaDamage();

	void LateDestroy();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> AreaProjectileClass;

	FTimerHandle AreaTimer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AreaDamageTime;

	UFUNCTION(BlueprintCallable)
	void ProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
	void Impact(FVector location, FRotator rotation);
		
};

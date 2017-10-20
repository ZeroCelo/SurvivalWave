// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LifeStat.h"
#include "ItemDropStat.h"
#include "Projectile.h"

#include "EnemyDrone.generated.h"

UCLASS()
class SURVIVALWAVE_API AEnemyDrone : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyDrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	class ULifeStat* LifeStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
	class UItemDropStat* DropStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* AttackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackIntervalMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackIntervalMax;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyDistanceMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyDistanceMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MinRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeDistMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeDistMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	float DyingTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Life")
	bool bIsDying;

	UFUNCTION(BlueprintCallable)
	void Attack(ACharacter* Char, FVector off);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFly(float pos);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateHUDLife();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DropItem(FTransform trans, FItem drop);

	UFUNCTION(BlueprintCallable)
	void DropItems();

	UFUNCTION(BlueprintCallable)
	void DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DeathBP();

	UFUNCTION(BlueprintCallable)
	void DetectDeath();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetKilled();

	FTimerHandle DeadTimer;
	FTimerHandle RefTimer;

	void UpdateVariables();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "LifeStat.h"
#include "ItemDropStat.h"

#include "EnemyCharacter.generated.h"

UCLASS()
class SURVIVALWAVE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateHUDLife();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateHUDShield();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DropItem(FTransform trans, FItem drop);

	UFUNCTION(BlueprintCallable)
	void DropItems();

	UFUNCTION(BlueprintCallable)
	void DetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void UnDetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void DetectDeath();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetKilled();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	float DyingTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Life")
	bool bIsDying;
	
	FTimerHandle DeadTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	class ULifeStat* LifeStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PerceptionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemDropStat* DropStats;

	//Returns a player in range, if any
	UFUNCTION(BlueprintCallable)
	AActor* GetTargetPlayer();

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<int64, AActor*> PlayerActors;
	TMap<int64, AActor*> PlayerActorsAttack;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void DetectAttack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void UnDetectAttack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void DoDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackInterval;

	UPROPERTY(EditAnywhere,Category = "Attack")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UBoxComponent* DamageBox;

	FTimerHandle AttackTimer;
};

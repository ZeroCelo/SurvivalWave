// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DamageStat.h"

#include "LifeStat.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALWAVE_API ULifeStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Life;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float LifeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ShieldMax;

	//Whether we use shield recharging functions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bShieldActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ShieldRechargeAmount;

	//Interval in wich shield recovers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ShieldRechargeRate;

	//Time the sield takes to start recharging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ShieldRechargeWait;

	bool bRecharging;

	FTimerHandle RechargeTimer;

	UFUNCTION(BlueprintCallable)
	bool DoesShieldRecharge() { return bShieldActive; }

	UFUNCTION(BlueprintCallable)
	bool IsShieldRecharging();

	UFUNCTION(BlueprintCallable)
	bool IsShieldActive();

	UFUNCTION(BlueprintCallable)
	float GetLife() { return Life; }

	UFUNCTION(BlueprintCallable)
	float GetLifeMax() { return LifeMax; }

	UFUNCTION(BlueprintCallable)
	float GetShield() { return Shield; }

	UFUNCTION(BlueprintCallable)
	float GetShieldMax() { return ShieldMax; }

	UFUNCTION(BlueprintCallable)
	void SetShieldActive(bool bActive);

	UFUNCTION(BlueprintCallable)
	bool NeedHealing();

	UFUNCTION(BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintCallable)
	void ShieldRecharge();

	//Function that calls the recharging procedure, true = Call in Waiting time, false = call in Recharging time
	UFUNCTION(BlueprintCallable)
	void ShieldCallRecharge(bool bWait = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamageMultiplier(float dmg, float multi);
	virtual void TakeDamageMultiplier_Implementation(float dmg, float multi);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamage(UDamageStat* dmg);
	virtual void TakeDamage_Implementation(UDamageStat* dmg);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RestoreLife(float rest);
	virtual void RestoreLife_Implementation(float rest);

	UFUNCTION(BlueprintCallable)
	void DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

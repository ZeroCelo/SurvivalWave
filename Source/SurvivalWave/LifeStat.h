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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Life;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeMax;

	UFUNCTION(BlueprintCallable)
	float GetLife() { return Life; }

	UFUNCTION(BlueprintCallable)
	float GetLifeMax() { return LifeMax; }

	UFUNCTION(BlueprintCallable)
	bool NeedHealing();

	UFUNCTION(BlueprintCallable)
	bool IsDead();

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

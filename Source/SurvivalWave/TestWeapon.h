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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float damage;

	//In Unreal units
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float damage_range;

	//Firing rate in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float damage_rate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float damage_ammo_cap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire")
	float damage_ammo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire")
	FTimerHandle FireTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire")
	bool hitting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire")
	FVector HitLocation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	//float damage_ammo_total;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	//float damage_ammo_total_cap;

	//UFUNCTION(BlueprintImplementableEvent)
	void StartFire();
	void StopFire();
	void Shoot();
};

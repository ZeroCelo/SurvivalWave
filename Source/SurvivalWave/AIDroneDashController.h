// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyDrone.h"

#include "AIDroneDashController.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDashState : uint8
{
	DS_Moving	UMETA(DisplayName = "DroneMoving"),
	DS_Charge	UMETA(DisplayName = "DroneCharging"),
	DS_Dash	UMETA(DisplayName = "DroneDash"),
	DS_Recharge	UMETA(DisplayName = "DroneRecharging"),
};

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AAIDroneDashController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AAIDroneDashController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	AEnemyDrone* EnemyRef;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyTimeMin;

	//Charging Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyTimeMax;
	
	//Min Time to start Dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeTimeMin;
	
	//Max Time to start Dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeTimeMax;

	//Min Distance to player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeDistMin;

	//Dash Distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeDistMax;

	//Walk Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyDistanceMin;

	//Dash Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyDistanceMax;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	//bool bIsCharging;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	//bool bIsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EDashState DroneState;
	
	FTimerHandle RefTimer;
	FTimerHandle StrafeTimer;
	FTimerHandle CheckTimer;

	void SetupMove();
	void SetupDash();

	void InitMove(bool bYes);
	void InitCharge();
	void InitDash(bool bYes);

	void ChargeDrone();
	void MoveDrone();
	void DashDrone();
	void DashCheck();

	void UpdateVariables();
	void MoveToLocation(FVector new_loc);
	void EnemyRefErrorCheck();
};

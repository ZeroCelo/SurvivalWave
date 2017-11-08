// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyDrone.h"

#include "AIDroneController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AAIDroneController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AAIDroneController(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	AEnemyDrone* EnemyRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackIntervalMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackIntervalMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MinRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyDistanceMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FlyDistanceMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeDistMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StrafeDistMax;

	float FlyHeight;

	UFUNCTION(BlueprintCallable)
	float GetHeight();

	FTimerHandle RefTimer;
	FTimerHandle DistTimer;
	FTimerHandle StrafeTimer;
	FTimerHandle AttackTimer;

	FVector TargetLocation;
	FVector LastPawnLocation;

	int32 StillCount;

	FVector ReturnLocationInRange(FVector pos);
	FVector StrafeDrone();
	FVector RepositionDrone();

	bool IsPointReachable(FVector Position);

	void UpdateVariables(); 
	void MoveToLocation(FVector new_loc);
	void EnemyRefErrorCheck();
	void FlyDrone();
	void MoveDrone();
	void CheckTargetDist();
	void Attack();
};

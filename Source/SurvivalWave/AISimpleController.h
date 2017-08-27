// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyCharacter.h"

#include "AISimpleController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AAISimpleController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AAISimpleController(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void CheckDistance();

	UFUNCTION(BlueprintCallable)
	void FindRandomTarget();

	UFUNCTION(BlueprintCallable)
	void CallFindTarget();

	UFUNCTION(BlueprintCallable)
	void CallCheckDistance();

	void EnemyRefErrorCheck();

	FTimerHandle RefTimer;
	FTimerHandle ErrorTimer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	AEnemyCharacter* EnemyRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	FVector Target;

	//How much time until the AI search for a new target to move. Used to avoid Stuck bugs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float NewTargetTolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float DistanceMargin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float DistanceFindRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float IdleWaitTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float IdleWaitTimeMax;

	//Time in Seconds for the CheckDistance function to loop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float IdleCheckTime;

	FTimerHandle IdleTimer;
};

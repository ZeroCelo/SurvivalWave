// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
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
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	FVector Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float DistanceMargin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float DistanceFindRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float IdleWaitTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float IdleWaitTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float IdleCheckTime;

	FTimerHandle IdleTimer;
	
};

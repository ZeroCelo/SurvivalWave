// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AISimpleController.h"
#include "AIFollowerController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AAIFollowerController : public AAISimpleController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AAIFollowerController(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void CheckState();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	AActor* TargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsIdle;

	//Time in Seconds to check the current state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StateTime;

	//Time in Seconds that the AI will remain still after reaching the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float DumbTime;

	FTimerHandle StateTimer;
};

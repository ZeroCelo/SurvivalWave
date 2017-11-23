// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SurvivalWaveCharacter.h"

#include "AIDroneFriendController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AAIDroneFriendController : public AAIController
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AAIDroneFriendController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	APawn* PawnRef;
	ASurvivalWaveCharacter* PlayerRef;

	FTimerHandle RefTimer;
	FTimerHandle FollowTimer;

	void EnemyRefErrorCheck();
	void UpdatePosition();
	
	
};

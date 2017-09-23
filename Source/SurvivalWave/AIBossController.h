// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SurvivalWaveCharacter.h"
#include "BossEnemyCharacter.h"

#include "AIBossController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AAIBossController : public AAIController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AAIBossController(const FObjectInitializer& ObjectInitializer);
	
	float StopTimeMin;
	float StopTimeMax;

	float SpawnTimeMin;
	float SpawnTimeMax;

	float ProjectileTimeMin;
	float ProjectileTimeMax;
	float ProjectileTimeInterval;

	int32 ProjectileMax;
	int32 ProjectileCount;

	//FVector ProjectileArrow;

	FTimerHandle RefTimer;
	FTimerHandle StopTimer;
	FTimerHandle SpawnTimer;
	FTimerHandle ProjectileTimer;

	ASurvivalWaveCharacter* PlayerRef;
	ABossEnemyCharacter* EnemyRef;
	
	void RefErrorCheck();
	void MoveBoss();
	void StopBoss();
	void SpawnAttack();
	void ProjectileAttack();
	void ProjectileSubAttack();
};

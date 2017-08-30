// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spawner.h"
#include "ItemDropStat.h"

#include "ItemSpawner.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API AItemSpawner : public ASpawner
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemDropStat* DropStats;
	
	virtual void PostSpawnActor(AActor* Act);
	void SpawnPoints();
};

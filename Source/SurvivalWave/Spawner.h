// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"
#include "Components/ArrowComponent.h"

#include "Spawner.generated.h"

UCLASS()
class SURVIVALWAVE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Returns the WhereToSpawn subobject
	FORCEINLINE UBoxComponent* GetWhereToSpawn() const { return SpawnBox; }

	//Fina a random point within the box component
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();

	//this function toggles whether or not to spawn
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawningActive(bool bShouldSpawn);

	//The pickup to Spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AActor> WhatToSpawn;

	FTimerHandle SpawnTimer;

	//Minimum/Maximum Spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayMax;

	//Box component to specify where pickups spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SpawnBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SpawnArrow;

	//Whether to spawn actors when game begins or this actor spawns in world
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	bool bStartSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	bool bRotateSpawn;

	//Wether the actors should follow this spawner rotation when spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	bool bRotateFollow;

	//Maximun number Spawn Actors before deactivating, negative values infinitely spawn, 0 deactivates completely
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxSpawnCount;

	//Number of actors to spawn each time the spawning happens
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 SpawnBurst;

	int32 CurrentSpawnCount;

	bool IsActive();
	bool IsInfinite();

	void SpawnStuff();

	//Handle the spawning of new pickup
	void SpawnActor();

	virtual void PostSpawnActor(AActor* Act);

	void CallSpawn();

	//The current spawn delay
	float SpawnDelay;
	
};

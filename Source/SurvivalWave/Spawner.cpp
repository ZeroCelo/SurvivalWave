// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "Spawner.h"
#include "EngineGlobals.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create the box component that represent the spawn volume
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnBox;

	//Set the spawn delays
	SpawnDelayMin = 2.0f;
	SpawnDelayMax = 3.5f;

	bStartSpawn = false;
	bRotateSpawn = true;

	CurrentSpawnCount = 0;
	MaxSpawnCount = 3;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bStartSpawn)
		CallSpawn();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawner::GetRandomPointInVolume() {
	FVector SpawnOrigin = SpawnBox->Bounds.Origin;
	FVector SpawnExtent = SpawnBox->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void ASpawner::SetSpawningActive(bool bShouldSpawn) {
	if (bShouldSpawn) {
		CurrentSpawnCount = 0;
		CallSpawn();
	}
	else {
		//clear the timer on spawn pickup
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}

void ASpawner::CallSpawn() {
	//set the timer on spawn pickup
	SpawnDelay = FMath::FRandRange(SpawnDelayMin, SpawnDelayMax);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawner::SpawnActor, SpawnDelay, false);
}

void ASpawner::PostSpawnActor(AActor* Act) {

}

void ASpawner::SpawnActor() {
	bool canSpawn = (MaxSpawnCount < 0 || CurrentSpawnCount < MaxSpawnCount);

	//If we have set something to spawn:
	if (WhatToSpawn != nullptr && canSpawn) {
		//check for a valid World:
		UWorld* const World = GetWorld();

		if (World != nullptr) {
			//set the spawn parameters
			FActorSpawnParameters SpawnParams;
			//SpawnParams.Owner = this;
			//SpawnParams.Instigator = Instigator;

			//Get a random location to spawn at
			FVector SpawnLocation = GetRandomPointInVolume();

			//get a random rotation for the spawned item
			FRotator SpawnRotation(0.0f);
			if (bRotateSpawn) {
				SpawnRotation.Yaw = FMath::FRand() * 360.0f;
				SpawnRotation.Pitch = FMath::FRand() * 360.0f;
				SpawnRotation.Roll = FMath::FRand() * 360.0f;
			}

			AActor* actor = World->SpawnActor<AActor>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			ACharacter* paw = Cast<ACharacter>(actor);
			PostSpawnActor(actor);
			
			CurrentSpawnCount++;
			CallSpawn();
		}
	}
}

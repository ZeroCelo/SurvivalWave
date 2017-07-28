// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "Pickup.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//All pickup start active
	bIsActive = true;

	//Create static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;
	PickupMesh->bGenerateOverlapEvents = true;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Return active state
bool APickup::IsActive() {
	return bIsActive;
}

//Changes active state
void APickup::SetActive(bool NewPickupState) {
	bIsActive = NewPickupState;
}

void APickup::WasCollected_Implementation() {
	//Log a debug message
	FString PickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have collected %s"), *PickupDebugString);

}

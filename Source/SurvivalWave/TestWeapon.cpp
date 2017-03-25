// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "TestWeapon.h"


// Sets default values
ATestWeapon::ATestWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


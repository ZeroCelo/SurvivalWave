// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshSkel"));
	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("WeaponArrow"));

	RootComponent = SkelMesh;
	//BaseMesh->SetupAttachment(RootComponent);
	//SkelMesh->SetupAttachment(RootComponent);
	FirePoint->SetupAttachment(RootComponent);

	SkelMesh->SetSimulatePhysics(false);

	Damage = 10.0f;
	DamageRange = 5000.0f;
	DamageRate = 0.25f;
	Ammo = 30.0f;
	AmmoMax = 30.0f;
		
	muzzle_time = 0.1115f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::StartFire_Implementation() {}
void AWeapon::StopFire_Implementation() {}
void AWeapon::SimulateFire_Implementation() {}
void AWeapon::SimulateFireStop_Implementation() {}

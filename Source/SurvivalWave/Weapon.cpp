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

	DamageStats = CreateDefaultSubobject<UDamageStat>(TEXT("DamageComponent"));
	this->AddOwnedComponent(DamageStats);

	SkelMesh->SetSimulatePhysics(false);

	/*Damage = 10.0f;
	DamageRange = 5000.0f;
	DamageRate = 0.25f;*/
	Ammo = 30;
	AmmoMax = 30;

	bAmmoInfinite = false;
	bReloadInfinite = false;
	
	ReloadTime = 1.0f;
	muzzle_time = 0.233f;
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

EItemType AWeapon::GetWeaponAmmoType() {
	return WeaponAmmoType;
}

EItemType AWeapon::GetWeaponType() {
	return WeaponType;
}

float AWeapon::ReloadAmmo(float new_ammo) {
	float temp = Ammo + new_ammo;
	if (temp > AmmoMax) {
		Ammo = AmmoMax;
		temp -= AmmoMax;
	}
	else {
		Ammo = temp;
		temp -= Ammo;
	}
	return temp;
}

float AWeapon::GetWeaponAmmo() {
	return Ammo;
}

float AWeapon::GetWeaponAmmoMax() {
	return AmmoMax;
}

bool AWeapon::ShouldReload() {
	return Ammo < AmmoMax && !bAmmoInfinite; 
}

bool AWeapon::IsReloadInfinite() {
	return bReloadInfinite;
}

bool AWeapon::IsAmmoEmpty() {
	return Ammo <= 0;
}

bool AWeapon::IsAmmoFull() {
	return Ammo >= AmmoMax;
}

void AWeapon::StartFire_Implementation() {
	float game_time = GetWorld()->GetTimeSeconds();
	float dif_time = game_time - LastFireTime;
	if (dif_time >= DamageStats->GetDamageRate()) {
		if (!bAmmoInfinite) {
			int32 newAmmo = Ammo - 1;
			if (newAmmo >= 0) {
				Shoot();
				LastFireTime = game_time;
				SimulateFire();
				Ammo = newAmmo;
			}
		}
		else {
			Shoot();
			LastFireTime = game_time;
			SimulateFire();
		}
	}
	LastFireDiff = LastFireTime + DamageStats->GetDamageRate() - game_time;
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeapon::StartFire,LastFireDiff , false);
	GetWorld()->GetTimerManager().SetTimer(MuzzleTimer, this, &AWeapon::SimulateFireStop, muzzle_time, false);
}

/*void AWeapon::StartFire_Implementation() {
	if (!bAmmoInfinite) {
		int32 newAmmo = Ammo - 1;
		if (newAmmo >= 0) {
			Shoot();
			SimulateFire();
			Ammo = newAmmo;
		}
	}
	else {
		Shoot();
		SimulateFire();
	}
	//GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeapon::StartFire, LastFireTime + DamageStats->GetDamageRate() - game_time, false);
	GetWorld()->GetTimerManager().SetTimer(MuzzleTimer, this, &AWeapon::SimulateFireStop, muzzle_time, false);
}*/

void AWeapon::StopFire_Implementation() {
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	//GetWorld()->GetTimerManager().ClearTimer(MuzzleTimer);

	//SimulateFireStop();
}
void AWeapon::SimulateFire_Implementation() {
	if (MuzzleFX != nullptr && MuzzlePSC == nullptr) {
		//MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FirePoint, NAME_None,FirePoint->RelativeLocation,FirePoint->RelativeRotation);
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FirePoint);
		//MuzzlePSC->bAutoDestroy = false;
		//MuzzlePSC->bAllowRecycling = true;
		//MuzzlePSC->bOwnerNoSee = false;
		//MuzzlePSC->bOnlyOwnerSee = true;
	}
}
void AWeapon::SimulateFireStop_Implementation() {
	if (MuzzlePSC != nullptr) {
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = nullptr;
	}
}
void AWeapon::Shoot_Implementation() {}

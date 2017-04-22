// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "TestWeapon.h"

#include "Engine.h"


// Sets default values
ATestWeapon::ATestWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshBase"));
	SkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshSkel"));
	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("WeaponArrow"));

	RootComponent = BaseMesh;
	//BaseMesh->SetupAttachment(RootComponent);
	SkelMesh->SetupAttachment(RootComponent);
	FirePoint->SetupAttachment(RootComponent);

	damage = 10.0f;
	damage_range = 5000.0f;
	damage_rate = 0.25f;
	damage_ammo_cap = 30.0f;
	damage_ammo = damage_ammo_cap;
	//damage_ammo_total_cap = damage_ammo_cap*4.0f;
	//damage_ammo_total = damage_ammo_cap*2.0f;
	trace_debug = false;
	last_fire = 0.0f;
	muzzle_time = 0.1115f;
}

// Called when the game starts or when spawned
void ATestWeapon::BeginPlay()
{
	Super::BeginPlay();
	//damage_ammo = damage_ammo_cap;
}

// Called every frame
void ATestWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestWeapon::Shoot() {
	float game_time = GetWorld()->GetTimeSeconds();
	float dif_time = game_time - last_fire;
	if (dif_time >= damage_rate) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shoot")));
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;
		//RV_TraceParams.debug
		const FName TraceTag("MyTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		if (trace_debug)
			RV_TraceParams.TraceTag = TraceTag;

		//Re-initialize hit info
		FHitResult RV_Hit(ForceInit);

		FVector start = FirePoint->GetComponentTransform().GetLocation();
		FVector end = start + FirePoint->GetComponentTransform().GetRotation().Vector()*damage_range;

		//call GetWorld() from within an actor extending class
		GetWorld()->LineTraceSingleByChannel(
			RV_Hit,        //result
			start,    //start
			end, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);
		if (RV_Hit.bBlockingHit)
			ShootImpact(RV_Hit);
		//hitting = RV_Hit.bBlockingHit;
		//HitLocation = RV_Hit.Location;
		last_fire = GetWorld()->GetTimeSeconds();
		SimulateFire();
	}
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ATestWeapon::Shoot, last_fire + damage_rate - game_time, false);
	GetWorld()->GetTimerManager().SetTimer(MuzzleTimer, this, &ATestWeapon::SimulateFireStop, muzzle_time, false);
}

void ATestWeapon::StartFire() {
	Shoot();
	//GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ATestWeapon::Shoot, damage_rate, true);
}

void ATestWeapon::StopFire() {
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	GetWorld()->GetTimerManager().ClearTimer(MuzzleTimer);
	//hitting = false;
	//GetWorld()->GetTimeSeconds();
	SimulateFireStop();
}

void ATestWeapon::SimulateFire() {
	if (MuzzleFX != nullptr && MuzzlePSC == nullptr) {
		//MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FirePoint, NAME_None,FirePoint->RelativeLocation,FirePoint->RelativeRotation);
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FirePoint);
		//MuzzlePSC->bOwnerNoSee = false;
		//MuzzlePSC->bOnlyOwnerSee = true;
	}
}

void ATestWeapon::SimulateFireStop() {
	if (MuzzlePSC != nullptr)
	{
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = nullptr;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "Weapon_Instant.h"

AWeapon_Instant::AWeapon_Instant() :Super() {
	LastFireTime = 0.0f;
}

void AWeapon_Instant::ShootImpact(FHitResult HitResult) {
	if (TraceFX != nullptr) {
		//UParticleSystemComponent* shot = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TraceFX,FirePoint->GetComponentLocation(),FirePoint->GetComponentRotation());
		UParticleSystemComponent* shot = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceFX, HitResult.TraceStart);
		shot->SetVectorParameter(FName("ShockBeamEnd"),HitResult.TraceEnd);
		if (ImpactFX != nullptr && HitResult.bBlockingHit) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, HitResult.ImpactPoint,HitResult.ImpactNormal.Rotation());
		}
	}
}

void AWeapon_Instant::StartFire_Implementation() {
	float game_time = GetWorld()->GetTimeSeconds();
	float dif_time = game_time - LastFireTime;
	if (dif_time >= DamageRate) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shoot")));
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;
		//RV_TraceParams.debug
		//const FName TraceTag("MyTraceTag");
		//GetWorld()->DebugDrawTraceTag = TraceTag;
		//if (trace_debug)
		//	RV_TraceParams.TraceTag = TraceTag;

		//Re-initialize hit info
		FHitResult RV_Hit(ForceInit);

		FVector start = FirePoint->GetComponentTransform().GetLocation();
		FVector end = start + FirePoint->GetComponentTransform().GetRotation().Vector()*DamageRange;

		//call GetWorld() from within an actor extending class
		GetWorld()->LineTraceSingleByChannel(
			RV_Hit,        //result
			start,    //start
			end, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);
		ShootImpact(RV_Hit);
		//hitting = RV_Hit.bBlockingHit;
		//HitLocation = RV_Hit.Location;
		LastFireTime = game_time;
		SimulateFire();
	}
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeapon_Instant::StartFire, LastFireTime + DamageRate - game_time, false);
	GetWorld()->GetTimerManager().SetTimer(MuzzleTimer, this, &AWeapon_Instant::SimulateFireStop, muzzle_time, false);
}

void AWeapon_Instant::StopFire_Implementation() {
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	//GetWorld()->GetTimerManager().ClearTimer(MuzzleTimer);
	
	//SimulateFireStop();
}

void AWeapon_Instant::SimulateFire_Implementation() {
	if (MuzzleFX != nullptr && MuzzlePSC == nullptr) {
		//MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FirePoint, NAME_None,FirePoint->RelativeLocation,FirePoint->RelativeRotation);
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, FirePoint);
		//MuzzlePSC->bAutoDestroy = false;
		//MuzzlePSC->bAllowRecycling = true;
		//MuzzlePSC->bOwnerNoSee = false;
		//MuzzlePSC->bOnlyOwnerSee = true;
	}
	
}

void AWeapon_Instant::SimulateFireStop_Implementation() {
	if (MuzzlePSC != nullptr)
	{
		MuzzlePSC->DeactivateSystem();
		MuzzlePSC = nullptr;
	}
}

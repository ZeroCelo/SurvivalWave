// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "LifeStat.h"
#include "EnemyCharacter.h"
#include "SurvivalWaveCharacter.h"
#include "Weapon_Instant.h"

AWeapon_Instant::AWeapon_Instant() :Super() {
	LastFireTime = 0.0f;
}

void AWeapon_Instant::ShootImpact(FHitResult HitResult) {
	if (TraceFX != nullptr) {
		//UParticleSystemComponent* shot = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TraceFX,FirePoint->GetComponentLocation(),FirePoint->GetComponentRotation());
		UParticleSystemComponent* shot = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceFX, HitResult.TraceStart);
		//shot->SetVectorParameter(FName("ShockBeamEnd"), HitResult.TraceEnd);
		
		if (HitResult.bBlockingHit) {
			if(ImpactFX != nullptr)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, HitResult.ImpactPoint,HitResult.ImpactNormal.Rotation());
			shot->SetVectorParameter(FName("ShockBeamEnd"), HitResult.ImpactPoint);
			DetectDamage(HitResult.GetActor());
		}
		else {
			shot->SetVectorParameter(FName("ShockBeamEnd"), HitResult.TraceEnd);
		}
	}
}

void AWeapon_Instant::Shoot_Implementation() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shoot")));
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.bIgnoreBlocks = false;
	//RV_TraceParams.debug
	//const FName TraceTag("MyTraceTag");
	//GetWorld()->DebugDrawTraceTag = TraceTag;
	//if (trace_debug)
		//RV_TraceParams.TraceTag = TraceTag;
	
	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	FVector start = FirePoint->GetComponentTransform().GetLocation();
	FVector end = start + FirePoint->GetComponentTransform().GetRotation().Vector()*DamageStats->GetDamageRange();

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
}

void AWeapon_Instant::DetectDamage(AActor* Act) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Weapon Detect")));
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Act);
	if (Enemy != nullptr) {
		Enemy->LifeStats->TakeDamage(DamageStats);
		Enemy->UpdateHUDLife();
	}
	ASurvivalWaveCharacter* Player = Cast<ASurvivalWaveCharacter>(Act);
	if (Player != nullptr) {
		Player->LifeStats->TakeDamage(DamageStats);
		Player->UpdateHUDLife();
	}
}

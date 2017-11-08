// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "AIDroneDashController.h"
#include "AI/Navigation/NavigationPath.h"
#include "Kismet/GameplayStatics.h"

#include "EngineGlobals.h"

AAIDroneDashController::AAIDroneDashController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {

	StrafeTimeMin = 1.5f;
	StrafeTimeMax = 2.5f;
	StrafeDistMin = 200.0f;
	StrafeDistMax = 600.0f;
	FlyDistanceMin = 600.0f;
	FlyDistanceMax = 2000.0f;
	FlyTimeMin = 2.0f;
	FlyTimeMax = 4.0f;
	//bIsCharging = false;
	//bIsMoving = true;
	DroneState = EDashState::DS_Recharge;
}

// Called when the game starts or when spawned
void AAIDroneDashController::BeginPlay()
{
	Super::BeginPlay();


	//GetWorld()->GetTimerManager().SetTimer(DistTimer, this, &AAIDroneController::CheckTargetDist, DistTime, true);
	GetWorld()->GetTimerManager().SetTimer(RefTimer, this, &AAIDroneDashController::EnemyRefErrorCheck, 0.100f, false);
	InitCharge();
}

void AAIDroneDashController::EnemyRefErrorCheck() {
	EnemyRef = Cast<AEnemyDrone>(GetPawn());
	if (EnemyRef != nullptr) {
		UpdateVariables();
		GetWorld()->GetTimerManager().ClearTimer(RefTimer);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Refing Ueah...")));
	}
	

}

void AAIDroneDashController::UpdateVariables() {
	AEnemyDrone* Drone = Cast<AEnemyDrone>(GetPawn());
	if (Drone != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Updating...")));
		StrafeTimeMin = Drone->StrafeTimeMin;
		StrafeTimeMax = Drone->StrafeTimeMax;
		StrafeDistMin = Drone->StrafeDistMin;
		StrafeDistMax = Drone->StrafeDistMax;
		FlyDistanceMin = Drone->FlyDistanceMin;
		FlyDistanceMax = Drone->FlyDistanceMax;
		FlyTimeMin = Drone->FlyTimeMin;
		FlyTimeMax = Drone->FlyTimeMax;
	}
}

void AAIDroneDashController::MoveToLocation(FVector new_loc) {
	/*FVector dist = GetPawn()->GetActorLocation() - LastPawnLocation;

	if (dist.Size() <= 100.0f)StillCount++;
	else StillCount = 0;

	LastPawnLocation = GetPawn()->GetActorLocation();
	TargetLocation = new_loc;*/
	GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, new_loc);
}

void AAIDroneDashController::InitMove(bool bYes) {
	if (bYes) {
		DroneState = EDashState::DS_Moving;
		GetWorld()->GetTimerManager().SetTimer(StrafeTimer, this, &AAIDroneDashController::MoveDrone, 0.200f, true);
	}
	else
		GetWorld()->GetTimerManager().ClearTimer(StrafeTimer);
}

void AAIDroneDashController::InitDash(bool bYes) {
	if (bYes) {
		//bIsCharging = false;
		//bIsMoving = false;
		DroneState = EDashState::DS_Charge;
		//GetWorld()->GetTimerManager().SetTimer(StrafeTimer, this, &AAIDroneDashController::DashDrone, 0.2f, true);
		float NextDash = FMath::RandRange(StrafeTimeMin, StrafeTimeMax);
		GetWorld()->GetTimerManager().SetTimer(StrafeTimer, this, &AAIDroneDashController::DashDrone, NextDash, false);
	}
	else
		GetWorld()->GetTimerManager().ClearTimer(StrafeTimer);
}

void AAIDroneDashController::InitCharge() {
	//bIsCharging = true;
	//bIsMoving = false;
	GetWorld()->GetTimerManager().ClearTimer(StrafeTimer);
	GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
	GetWorld()->GetTimerManager().SetTimer(StrafeTimer, this, &AAIDroneDashController::ChargeDrone, FlyTimeMax, false);
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &AAIDroneDashController::DashCheck, 0.15f, true);
}

void AAIDroneDashController::DashCheck() {
	if (EnemyRef != nullptr) {
		if (EnemyRef->GetCharacterMovement()->Velocity.Size() < 0.1f) {
			GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			DroneState = EDashState::DS_Recharge;
		}
		else {
			DroneState = EDashState::DS_Dash;
		}
	}
}

void AAIDroneDashController::ChargeDrone() {
	InitMove(false);
	InitMove(true);
}

void AAIDroneDashController::SetupMove() {
	if (EnemyRef != nullptr) {
		EnemyRef->GetCharacterMovement()->MaxWalkSpeed = FlyDistanceMin;
		DroneState = EDashState::DS_Moving;
		//bIsCharging = false;
		//bIsMoving = true;
	}
}

void AAIDroneDashController::MoveDrone() {
	if (EnemyRef != nullptr) {
		if (!EnemyRef->LifeStats->IsDead()) {
			SetupMove();
			ACharacter* play = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (play != nullptr) {
				FVector dist = play->GetActorLocation() -  EnemyRef->GetActorLocation();
				if (dist.Size() > StrafeDistMin)
					MoveToLocation(play->GetActorLocation());
				else {
					MoveToLocation(EnemyRef->GetActorLocation());
					InitDash(false);
					InitDash(true);
				}
			}
		}
	}
}

void AAIDroneDashController::SetupDash() {
	if (EnemyRef != nullptr) {
		EnemyRef->GetCharacterMovement()->MaxWalkSpeed = FlyDistanceMax;
		DroneState = EDashState::DS_Dash;
		//bIsCharging = false;
		//bIsMoving = false;
	}
}

void AAIDroneDashController::DashDrone() {
	if (EnemyRef != nullptr) {
		if (!EnemyRef->LifeStats->IsDead()) {
			SetupDash();
			ACharacter* play = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (play != nullptr) {
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

				FVector dir = play->GetActorLocation() - EnemyRef->GetActorLocation();
				dir.Normalize();

				FVector start = EnemyRef->GetActorLocation();// +FVector(0.0f, 0.0f, 50.0f);
				FVector end = start + (dir*StrafeDistMax);

				/*GetWorld()->LineTraceSingleByChannel(
					RV_Hit,        //result
					start,    //start
					end, //end
					ECC_WorldStatic, //collision channel
					RV_TraceParams
				);*/

				GetWorld()->LineTraceSingleByObjectType(
					RV_Hit,        //result
					start,    //start
					end, //end
					ECC_WorldStatic, 
					RV_TraceParams
				);
				
				
				/* //Aqui Ta dando ruim no trace
				//FCollisionObjectQueryParams param(ECC_TO_BITFIELD(ECC_WorldStatic));
				//param.DoVerify();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Tracing...")));
				GetWorld()->SweepSingleByObjectType(
					RV_Hit,
					start,
					end,
					FQuat(),
					param,
					play->GetCapsuleComponent()->GetCollisionShape(),
					RV_TraceParams
				);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Tracing...2")));
				*/
				
				FVector loc = end;
				if (RV_Hit.bBlockingHit) {
					loc = RV_Hit.ImpactPoint - dir*50.0f;
				}

				MoveToLocation(loc);
				InitCharge();
			}
		}
	}
}

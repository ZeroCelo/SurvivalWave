// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "AIDroneController.h"

#include "AI/Navigation/NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "EngineGlobals.h"


AAIDroneController::AAIDroneController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {

	MinRange = 350.0f;
	MaxRange = 500.0f;
	FlyTimeMin = 1.75f;
	FlyTimeMax = 3.0f;
	FlyDistanceMin = 100.0f;
	FlyDistanceMax = 200.0f;
	StrafeTimeMin = 1.5f;
	StrafeTimeMax = 2.5f;
	StrafeDistMin = 200.0f;
	StrafeDistMax = 400.0f;
	AttackIntervalMin = 2.0f;
	AttackIntervalMax = 3.0f;
	StillCount = 0;
}

// Called when the game starts or when spawned
void AAIDroneController::BeginPlay()
{
	Super::BeginPlay();


	//GetWorld()->GetTimerManager().SetTimer(DistTimer, this, &AAIDroneController::CheckTargetDist, DistTime, true);
	GetWorld()->GetTimerManager().SetTimer(StrafeTimer, this, &AAIDroneController::MoveDrone, StrafeTimeMin, false);
	GetWorld()->GetTimerManager().SetTimer(DistTimer, this, &AAIDroneController::FlyDrone, FlyTimeMin, false);
	GetWorld()->GetTimerManager().SetTimer(RefTimer, this, &AAIDroneController::EnemyRefErrorCheck, 0.100f, false);
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AAIDroneController::Attack, AttackIntervalMax, false);
}

void AAIDroneController::EnemyRefErrorCheck() {
	EnemyRef = Cast<AEnemyDrone>(GetPawn());
	if (EnemyRef != nullptr) {
		GetWorld()->GetTimerManager().ClearTimer(RefTimer);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Refing Ueah...")));
	}
}

float AAIDroneController::GetHeight() {
	return FlyHeight;
}

void AAIDroneController::FlyDrone() {
	if (EnemyRef != nullptr) {
		if (!EnemyRef->LifeStats->IsDead()) {
			FlyHeight = FMath::RandRange(FlyDistanceMin, FlyDistanceMax);
			if (EnemyRef != nullptr)EnemyRef->UpdateFly(FlyHeight);
			float NextFly = FMath::RandRange(FlyTimeMin, FlyTimeMax);
			GetWorld()->GetTimerManager().SetTimer(DistTimer, this, &AAIDroneController::FlyDrone, NextFly, false);
		}
	}
}

void AAIDroneController::MoveToLocation(FVector new_loc) {
	FVector dist = GetPawn()->GetActorLocation() - LastPawnLocation;
	
	if (dist.Size() <= 100.0f)StillCount++;
	else StillCount = 0;
	
	LastPawnLocation = GetPawn()->GetActorLocation();
	TargetLocation = new_loc;
	GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, TargetLocation);
}

void AAIDroneController::CheckTargetDist() {
	ACharacter* play = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if (play != nullptr) {
		FVector dist = GetPawn()->GetActorLocation() - play->GetActorLocation();
		if (dist.Size() >= MaxRange) {
			dist.Normalize();
			dist *= MaxRange;
			FVector new_location = play->GetActorLocation() + dist;
			//GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, new_location);
			MoveToLocation(new_location);
		}
		else if (dist.Size() <= MinRange) {
			dist.Normalize();
			dist *= MinRange;
			FVector new_location = play->GetActorLocation() + dist;
			//GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, new_location);
			MoveToLocation(new_location);
		}
	}
}

bool AAIDroneController::IsPointReachable(FVector Position) {
	FVector PathStart = GetPawn()->GetActorLocation();
	UNavigationPath* NavPath = UNavigationSystem::FindPathToLocationSynchronously(GetWorld(), PathStart, Position, NULL);

	if (!NavPath)
		return false;

	return !NavPath->IsPartial();
}

FVector AAIDroneController::ReturnLocationInRange(FVector pos) {
	ACharacter* play = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (play != nullptr) {
		FVector dist = pos - play->GetActorLocation();
		if (dist.Size() >= MaxRange) {
			float diff = dist.Size() - MaxRange;
			dist.Normalize();
			dist *= -diff;
			return pos + dist;
		}
		else if (dist.Size() <= MinRange) {
			float diff = MinRange - dist.Size();
			dist.Normalize();
			dist *= diff;
			return pos + dist;
		}
	}
	return pos;
}

FVector AAIDroneController::StrafeDrone() {
	ACharacter* play = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (play != nullptr) {
		FVector UpVect(0.0f, 0.0f, 1.0f);
		FVector dist = GetPawn()->GetActorLocation() - play->GetActorLocation();
		dist.Normalize();
		FVector RightVect = FVector::CrossProduct(dist, UpVect);
		RightVect.Normalize();

		float pos = FMath::RandRange(StrafeDistMin, StrafeDistMax);
		int32 neg = FMath::RandRange(0, 1);

		RightVect *= pos*(FMath::Cos(neg * 180));

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("New Strafe %f, %f, %f"), RightVect.X, RightVect.Y, RightVect.Z));

		FVector new_location = GetPawn()->GetActorLocation() + RightVect;
		new_location = ReturnLocationInRange(new_location);
		return new_location;
	}

	return GetPawn()->GetActorLocation();
}

FVector AAIDroneController::RepositionDrone() {
	if (StillCount > 1) {
		FNavLocation Result;
		GetWorld()->GetNavigationSystem()->GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(),StrafeDistMax,Result);
		
		return Result.Location;
	}
	else {
		ACharacter* play = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (play != nullptr) {
			FVector dist = play->GetActorLocation() - GetPawn()->GetActorLocation();
			float d = dist.Size();
			dist.Normalize();
			//dist *= (d*4.0f);
			dist *= MaxRange + d;

			//FVector new_location = GetPawn()->GetActorLocation() + dist;
			FVector new_location = play->GetActorLocation() + dist;
			return new_location;
		}
		else
			return GetPawn()->GetActorLocation();
	}
}

void AAIDroneController::MoveDrone() {
	if (EnemyRef != nullptr) {
		if (!EnemyRef->LifeStats->IsDead()) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Strafing...")));
			FVector new_location = StrafeDrone();
			if (StillCount > 0) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Oopss %d"), StillCount));
				new_location = RepositionDrone();
			}

			MoveToLocation(new_location);

			float NextStrafe = FMath::RandRange(StrafeTimeMin, StrafeTimeMax);
			GetWorld()->GetTimerManager().SetTimer(StrafeTimer, this, &AAIDroneController::MoveDrone, NextStrafe, false);
		}
	}
}

void AAIDroneController::Attack() {
	if (EnemyRef != nullptr) {
		if (!EnemyRef->LifeStats->IsDead()) {
			ACharacter* play = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (play != nullptr) {
				EnemyRef->Attack(play);
			}

			float NextAttack = FMath::RandRange(AttackIntervalMin, AttackIntervalMax);
			GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AAIDroneController::Attack, NextAttack, false);
		}
	}
}

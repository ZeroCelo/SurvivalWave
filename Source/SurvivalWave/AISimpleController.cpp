// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "AISimpleController.h"

#include "EngineGlobals.h"

AAISimpleController::AAISimpleController(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {

	IdleCheckTime = 0.050f;
	IdleWaitTimeMin = 3.0f;
	IdleWaitTimeMax = 4.0f;
	DistanceMargin = 200.0f;
	DistanceFindRadius = 1000.0f;
	NewTargetTolerance = 8.0f;
	EnemyRef = nullptr;
}

// Called when the game starts or when spawned
void AAISimpleController::BeginPlay()
{
	Super::BeginPlay();

	
	//GetWorld()->GetTimerManager().SetTimer(IdleCheckTimer, this, &AEnemyCharacter::CheckDistance, IdleCheckTime, true);
	GetWorld()->GetTimerManager().SetTimer(RefTimer, this, &AAISimpleController::EnemyRefErrorCheck, 0.100f, true);
	CallFindTarget();
}

void AAISimpleController::EnemyRefErrorCheck() {
	EnemyRef = Cast<AEnemyCharacter>(GetPawn());
	if (EnemyRef != nullptr){
		GetWorld()->GetTimerManager().ClearTimer(RefTimer);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Refing Ueah...")));
	}
	/*else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Refing NULL...")));
	}*/
}

void AAISimpleController::FindRandomTarget() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy Finding...")));
	FNavLocation Result;
	FVector ActorLocation = GetPawn()->GetActorLocation();
	GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(ActorLocation, DistanceFindRadius, Result);
	Target = Result.Location;
	GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, Target);
	CallCheckDistance();
}

void AAISimpleController::CheckDistance() {
	FVector ActorLocation = GetPawn()->GetActorLocation();
	FVector Dist = ActorLocation - Target;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy Checking %f"),Dist.Size()));
	if (Dist.Size() < DistanceMargin) {
		CallFindTarget();
	}
}

void AAISimpleController::CallCheckDistance() {
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
	GetWorld()->GetTimerManager().ClearTimer(ErrorTimer);
	GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &AAISimpleController::CheckDistance, IdleCheckTime, true);
	GetWorld()->GetTimerManager().SetTimer(ErrorTimer, this, &AAISimpleController::FindRandomTarget, NewTargetTolerance, false);
}

void AAISimpleController::CallFindTarget() {
	float TimeToWait = FMath::RandRange(IdleWaitTimeMin, IdleWaitTimeMax);
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
	GetWorld()->GetTimerManager().ClearTimer(ErrorTimer);
	GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &AAISimpleController::FindRandomTarget, TimeToWait, false);
}

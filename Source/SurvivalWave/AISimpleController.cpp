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

}

// Called when the game starts or when spawned
void AAISimpleController::BeginPlay()
{
	Super::BeginPlay();

	
	//GetWorld()->GetTimerManager().SetTimer(IdleCheckTimer, this, &AEnemyCharacter::CheckDistance, IdleCheckTime, true);
	CallFindTarget();
}

void AAISimpleController::FindRandomTarget() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy Finding...")));
	FNavLocation Result;
	FVector ActorLocation = GetControlledPawn()->GetActorLocation();
	GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(ActorLocation, DistanceFindRadius, Result);
	Target = Result.Location;
	GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, Target);
	CallCheckDistance();
}

void AAISimpleController::CheckDistance() {
	FVector ActorLocation = GetControlledPawn()->GetActorLocation();
	FVector Dist = ActorLocation - Target;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy Checking %f"),Dist.Size()));
	if (Dist.Size() < DistanceMargin) {
		CallFindTarget();
	}
}

void AAISimpleController::CallCheckDistance() {
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
	GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &AAISimpleController::CheckDistance, IdleCheckTime, true);
}

void AAISimpleController::CallFindTarget() {
	float TimeToWait = FMath::RandRange(IdleWaitTimeMin, IdleWaitTimeMax);
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
	GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &AAISimpleController::FindRandomTarget, TimeToWait, false);
}

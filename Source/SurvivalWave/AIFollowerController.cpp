// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "AIFollowerController.h"

#include "EngineGlobals.h"

AAIFollowerController::AAIFollowerController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {
	TargetActor = nullptr;
	StateTime = 0.050f;
	DumbTime = 2.00f;
	bIsIdle = true;
}

// Called when the game starts or when spawned
void AAIFollowerController::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(StateTimer, this, &AAIFollowerController::CheckState, StateTime, false);
}

void AAIFollowerController::CheckState() {
	float NextCall = StateTime;
	if (EnemyRef != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Ref...")));
		if (!EnemyRef->LifeStats->IsDead()) {
			TargetActor = EnemyRef->GetTargetPlayer();
			if (TargetActor != nullptr) {
				bIsIdle = false;
				GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
				Target = TargetActor->GetActorLocation();
				GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, Target);
				FVector dist = EnemyRef->GetActorLocation() - TargetActor->GetActorLocation();
				if (dist.Size() <= 100.0f) {
					//NextCall = FMath::RandRange(StrafeTimeMin, StrafeTimeMax);
					EnemyRef->DoDamage();
					NextCall = DumbTime;
				}
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("GET HIM!!")));
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("No Target")));
				if (!bIsIdle) {
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Finding...")));
					CallFindTarget();
					bIsIdle = true;
				}
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(StateTimer, this, &AAIFollowerController::CheckState, NextCall, false);
}

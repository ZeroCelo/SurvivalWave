// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "AIFollowerController.h"

#include "EngineGlobals.h"

AAIFollowerController::AAIFollowerController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {
	TargetActor = nullptr;
	StateTime = 0.050f;
	bIsIdle = true;
}

// Called when the game starts or when spawned
void AAIFollowerController::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(StateTimer, this, &AAIFollowerController::CheckState, StateTime, true);
}

void AAIFollowerController::CheckState() {
	if (EnemyRef != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Ref...")));
		if (!EnemyRef->LifeStats->IsDead()) {
			TargetActor = EnemyRef->GetTargetPlayer();
			if (TargetActor != nullptr) {
				bIsIdle = false;
				GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
				Target = TargetActor->GetActorLocation();
				GetWorld()->GetNavigationSystem()->SimpleMoveToLocation(this, Target);
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
}

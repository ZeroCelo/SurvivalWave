// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "AIBossController.h"

#include "AI/Navigation/NavigationPath.h"
#include "Kismet/GameplayStatics.h"


AAIBossController::AAIBossController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {

	StopTimeMin = 4.0f;
	StopTimeMax = 5.0f;

	SpawnTimeMin = 6.0f;
	SpawnTimeMax = 7.0f;

	ProjectileTimeMin = 4.0f;
	ProjectileTimeMax = 5.0f;
	ProjectileTimeInterval = 0.5f;
	ProjectileCount = 0;
	ProjectileMax = 5;
}

// Called when the game starts or when spawned
void AAIBossController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(RefTimer, this, &AAIBossController::RefErrorCheck, 0.100f, false);
	GetWorld()->GetTimerManager().SetTimer(StopTimer, this, &AAIBossController::MoveBoss, StopTimeMin, false);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AAIBossController::SpawnAttack, SpawnTimeMin, false);
	GetWorld()->GetTimerManager().SetTimer(ProjectileTimer, this, &AAIBossController::ProjectileAttack, ProjectileTimeMax, false);
}

void AAIBossController::RefErrorCheck() {
	EnemyRef = Cast<ABossEnemyCharacter>(GetPawn());
	if (EnemyRef != nullptr) {
		GetWorld()->GetTimerManager().ClearTimer(RefTimer);
		//ProjectileArrow = EnemyRef->ProjectilePoint->GetComponentLocation();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Refing Ueah...")));
	}
	PlayerRef = Cast<ASurvivalWaveCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerRef != nullptr) {
		GetWorld()->GetTimerManager().ClearTimer(RefTimer);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Refing Ueah...")));
	}
}

void AAIBossController::StopBoss() {
	if (EnemyRef != nullptr) {

		GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, EnemyRef);
		
		GetWorld()->GetTimerManager().ClearTimer(StopTimer);
		float time = FMath::FRandRange(StopTimeMin,StopTimeMax);
		GetWorld()->GetTimerManager().SetTimer(StopTimer, this, &AAIBossController::MoveBoss, time, false);
	}
	
}

void AAIBossController::MoveBoss() {
	if (PlayerRef != nullptr && EnemyRef != nullptr) {
		if (!EnemyRef->LifeStats->IsDead()) {
			GetWorld()->GetNavigationSystem()->SimpleMoveToActor(this, PlayerRef);

			float time = FMath::FRandRange(StopTimeMin, StopTimeMax);
			GetWorld()->GetTimerManager().SetTimer(StopTimer, this, &AAIBossController::StopBoss, time, false);
		}
	}
}

void AAIBossController::SpawnAttack() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Refing Ueah...")));
	if (EnemyRef != nullptr) {
		if (!EnemyRef->LifeStats->IsDead()) {
			EnemyRef->SpawnMinions();
			//GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
			float time = FMath::FRandRange(SpawnTimeMin, SpawnTimeMax);
			GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AAIBossController::SpawnAttack, time, false);
		}
	}
}

void AAIBossController::ProjectileSubAttack() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Projectile SubAttack %d"), ProjectileCount));
	if (EnemyRef != nullptr) {
		if (!EnemyRef->LifeStats->IsDead()) {
			if (ProjectileCount < ProjectileMax) {
				
				//-------Ta com bug isso aqui
				FVector EnemyPos = EnemyRef->GetActorLocation();
				FVector ArrowPos = EnemyRef->ProjectilePoint->GetComponentLocation();
				float radius = ArrowPos.Z - EnemyPos.Z;
				float amount = 180.0f / (ProjectileMax + 1.0f);
				float current = amount + ProjectileCount*amount;
				FVector NewPos;
				//NewPos.Z = radius*FMath::Sin(FMath::DegreesToRadians(45.0f));
				//NewPos.Y = radius*FMath::Cos(FMath::DegreesToRadians(45.0f));
				NewPos.Z = radius*FMath::Sin(FMath::DegreesToRadians(current));
				NewPos.Y = radius*FMath::Cos(FMath::DegreesToRadians(current));
				NewPos = EnemyPos + NewPos;
				//-------Ta com bug isso aqui

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("NewPosZ %f EnemyZ %f ArrowZ %f"), NewPos.Z, EnemyPos.Z,ArrowPos.Z));

				//EnemyRef->ProjectilePoint->SetWorldLocation(NewPos);
				EnemyRef->ProjectileAttack(NewPos, false);
				//EnemyRef->ProjectilePoint->SetWorldLocation(ProjectileArrow);

				++ProjectileCount;
				GetWorld()->GetTimerManager().SetTimer(ProjectileTimer, this, &AAIBossController::ProjectileSubAttack, ProjectileTimeInterval, false);
			}
			else {
				ProjectileCount = 0;
				float time = FMath::FRandRange(ProjectileTimeMin, ProjectileTimeMax);
				GetWorld()->GetTimerManager().ClearTimer(ProjectileTimer);
				GetWorld()->GetTimerManager().SetTimer(ProjectileTimer, this, &AAIBossController::ProjectileAttack, time, false);
			}
		}
	}
}

void AAIBossController::ProjectileAttack() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Projectile Attack")));
	GetWorld()->GetTimerManager().SetTimer(ProjectileTimer, this, &AAIBossController::ProjectileSubAttack, ProjectileTimeInterval, false);
}

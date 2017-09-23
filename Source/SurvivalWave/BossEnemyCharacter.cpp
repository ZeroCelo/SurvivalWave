// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "BossEnemyCharacter.h"
#include "SurvivalWaveGameMode.h"
#include "AIBossController.h"

#include "Kismet/GameplayStatics.h"
#include "AI/Navigation/NavigationPath.h"

#include "EngineGlobals.h"

ABossEnemyCharacter::ABossEnemyCharacter():Super(){

	IntimidateTimeMin = 6.0f;
	IntimidateTimeMax = 7.0f;

	ProjectilePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileArrow"));
	ProjectilePoint->SetupAttachment(RootComponent);

	StopTimeMin = 4.0f;
	StopTimeMax = 5.0f;

	SpawnTimeMin = 6.0f;
	SpawnTimeMax = 7.0f;

	ProjectileTimeMin = 4.0f;
	ProjectileTimeMax = 5.0f;
	ProjectileTimeInterval = 0.5f;
	ProjectileMax = 5;

}

// Called when the game starts or when spawned
void ABossEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateController();
	CallIntimidateIntro();
	//GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &ABossEnemyCharacter::SpawnMinions, 3.0f, true);
}

void ABossEnemyCharacter::UpdateController() {
	AAIBossController* con = Cast<AAIBossController>(GetController());
	if (con != nullptr) {
		con->StopTimeMin = StopTimeMin;
		con->StopTimeMax = StopTimeMax;

		con->SpawnTimeMin = SpawnTimeMin;
		con->SpawnTimeMax = SpawnTimeMax;

		con->ProjectileTimeMin = ProjectileTimeMin;
		con->ProjectileTimeMax = ProjectileTimeMax;
		con->ProjectileTimeInterval = ProjectileTimeInterval;
		con->ProjectileMax = ProjectileMax;
	}
}

void ABossEnemyCharacter::CallIntimidateIntro() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Calling...")));
	
	GetWorld()->GetTimerManager().SetTimer(IntimidateIntroTimer, this, &ABossEnemyCharacter::IntimidatePlayerIntro, 0.50f, false);
}

void ABossEnemyCharacter::CallIntimidate() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Calling...")));
	float time = FMath::RandRange(IntimidateTimeMin, IntimidateTimeMax);
	GetWorld()->GetTimerManager().SetTimer(IntimidateTimer, this, &ABossEnemyCharacter::IntimidatePlayer, time, false);
}

void ABossEnemyCharacter::IntimidatePlayerIntro() {
	if (!LifeStats->IsDead()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Intimidating...")));
		ASurvivalWaveGameMode* GM = Cast<ASurvivalWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GM != nullptr) {
			int32 msgIndex = FMath::RandRange(0, IntimidateMsgIntro.Num() - 1);

			if(msgIndex >= 0 || msgIndex < IntimidateMsgIntro.Num())
				GM->UpdateBossInfo(IntimidateMsgIntro[msgIndex].ToString());
			//GM->UpdateInfo(IntimidateMsg[msgIndex].ToString());

			CallIntimidate();
		}
	}
}

void ABossEnemyCharacter::IntimidatePlayer() {
	if (!LifeStats->IsDead()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Intimidating...")));
		ASurvivalWaveGameMode* GM = Cast<ASurvivalWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GM != nullptr) {
			int32 msgIndex = FMath::RandRange(0, IntimidateMsg.Num() - 1);
			while (IntimidatePreviousMsg.Find(msgIndex) != INDEX_NONE) {
				msgIndex = FMath::RandRange(0, IntimidateMsg.Num() - 1);
			}

			if (msgIndex >= 0 || msgIndex < IntimidateMsg.Num())
				GM->UpdateBossInfo(IntimidateMsg[msgIndex].ToString());
			//GM->UpdateInfo(IntimidateMsg[msgIndex].ToString());
			IntimidatePreviousMsg.Add(msgIndex);
			if (IntimidatePreviousMsg.Num() > 3) {
				IntimidatePreviousMsg.RemoveAt(0);
			}

			CallIntimidate();
		}
	}
}

void ABossEnemyCharacter::SpawnMinions() {
	if (!LifeStats->IsDead()) {
		for (auto& Minion : Minions) {
			int32 quant = FMath::RandRange(Minion.QuantityMin, Minion.QuantityMax);
			FNavLocation Result;

			for (int32 i = 0; i < quant; ++i) {
				if (Minion.MinionClass != nullptr) {
					//GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(GetActorLocation(), PerceptionSphere->GetScaledSphereRadius(), Result);
					GetWorld()->GetNavigationSystem()->GetRandomReachablePointInRadius(GetActorLocation(), PerceptionSphere->GetScaledSphereRadius(), Result);
					FRotator rot(0.0f);
					FActorSpawnParameters SpawnInfo;
					SpawnInfo.Owner = this;
					SpawnInfo.Instigator = Instigator;
					GetWorld()->SpawnActor<AActor>(Minion.MinionClass, Result.Location, rot, SpawnInfo);
				}
			}
		}
	}
}

void ABossEnemyCharacter::ProjectileAttack(FVector Offset, bool bUseOffset) {
	if (ProjectileAbilityClass != nullptr) {
		FVector location = ProjectilePoint->GetComponentLocation();
		FRotator rot(0.0f);
		if (bUseOffset)
			location = Offset;
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = Instigator;
		GetWorld()->SpawnActor<AActor>(ProjectileAbilityClass, location, rot, SpawnInfo);
	}
}

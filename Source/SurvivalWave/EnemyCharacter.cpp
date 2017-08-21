// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "EnemyCharacter.h"
#include "SurvivalWaveCharacter.h"

//#include "AI/Navigation/NavigationSystem.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineGlobals.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeStats = CreateDefaultSubobject<ULifeStat>(TEXT("LifeComponent"));
	this->AddOwnedComponent(LifeStats);

	PerceptionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PerceptionSphere"));
	PerceptionSphere->SetupAttachment(RootComponent);
	PerceptionSphere->SetSphereRadius(500.0f);

	PerceptionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::DetectPlayer);
	PerceptionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::UnDetectPlayer);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LifeStats->DetectDamage(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UpdateHUDLife();

}

AActor* AEnemyCharacter::GetTargetPlayer() {
	if (PlayerActors.Num()) {
		return PlayerActors.CreateConstIterator()->Value;
	}
	return nullptr;
}

void AEnemyCharacter::DetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ASurvivalWaveCharacter* play = Cast<ASurvivalWaveCharacter>(OtherActor);
	if (play != nullptr) {
		int64 add = (int64)(OtherActor);
		int64 poi = (int64)(&OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player >>Detect %s %llx %llX"),*play->GetName(),poi,add));
		if(!PlayerActors.Contains(add))
			PlayerActors.Add(add,OtherActor);
	}
}

void AEnemyCharacter::UnDetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ASurvivalWaveCharacter* play = Cast<ASurvivalWaveCharacter>(OtherActor);
	if (play != nullptr) {
		int64 add = (int64)(OtherActor);
		int64 poi = (int64)(&OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Detect>> %s %llx %llX"), *play->GetName(), poi, add));
		if (PlayerActors.Contains(add))
			PlayerActors.Remove(add);
	}
}

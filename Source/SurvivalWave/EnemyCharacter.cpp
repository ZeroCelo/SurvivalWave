// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "EnemyCharacter.h"

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

	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::DetectDamage);
	/*bStateIdle = true;
	IdleCheckTime = 0.050f;
	IdleWaitTimeMin = 3.0f;
	IdleWaitTimeMax = 4.0f;
	DistanceMargin = 200.0f;
	DistanceFindRadius = 1000.0f;
	*/
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

// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "EnemyDrone.h"

#include "Kismet/KismetMathLibrary.h"

#include "AIDroneController.h"


// Sets default values
AEnemyDrone::AEnemyDrone()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(48.0f);
	//CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyDrone::DetectDamage);


	LifeStats = CreateDefaultSubobject<ULifeStat>(TEXT("LifeComponent"));
	this->AddOwnedComponent(LifeStats);

	DropStats = CreateDefaultSubobject<UItemDropStat>(TEXT("DropComponent"));
	this->AddOwnedComponent(DropStats);

	AttackPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttackArrow"));
	AttackPoint->SetupAttachment(CollisionSphere);

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

	DyingTime = 5.0f;
	bIsDying = false;

}

// Called when the game starts or when spawned
void AEnemyDrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyDrone::DropItems() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Drop Stuff")));
	if (DropStats->GetDrop()) {
		int32 quant = DropStats->GetDropQuantity();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Drop Quant %d"),quant));
		for (int32 i = 0; i < quant; i++) {
			//FVector Origin = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
			FVector Origin = CollisionSphere->GetComponentLocation() + FVector(0.0f, 0.0f, 50.0f);
			FVector Extent(50.0f, 50.0f, 50.0f);
			FVector Result = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
			//Origin += Result + FVector(0.0f, 0.0f, 100.0f);
			FRotator Rot = FRotationMatrix::MakeFromX(Result).Rotator();
			FTransform Trans(Rot, Result);
			FItem ite = DropStats->GetDropItem();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Droping..%f,%f,%f"),Origin.X,Origin.Y,Origin.Z));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Drop =%s"), *FItem::GetItemEnumAsString(ite.type)));
			DropItem(Trans, ite);
		}
	}
	if(CollisionSphere != nullptr)
		DropStats->DropPoints(CollisionSphere->GetComponentLocation());
}

void AEnemyDrone::Death() {
	DeathBP();
	Destroy();
}

void AEnemyDrone::DetectDeath() {
	if (LifeStats->IsDead() && !bIsDying) {
		bIsDying = true;
		DropItems();
		GetKilled();
		GetWorld()->GetTimerManager().SetTimer(DeadTimer, this, &AEnemyDrone::Death, DyingTime, false);
	}
}

void AEnemyDrone::DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Drone Taken Damage")));
	LifeStats->DetectDamage(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UpdateHUDLife();
	UpdateHUDShield();
	DetectDeath();
}

void AEnemyDrone::Attack(ACharacter* Char, FVector off) {
	if (ProjectileClass != nullptr && Char != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Pew Pew")));
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = Instigator;
		FVector Direction = (Char->GetActorLocation() + off) - AttackPoint->GetComponentLocation();
		FRotator Rot = FRotationMatrix::MakeFromX(Direction).Rotator();

		AProjectile* proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, AttackPoint->GetComponentLocation(), Rot, SpawnInfo);
		//if (proj != nullptr) {
			//proj->DamageStats->CopyDamage(DamageStats);
		//}
	}
}

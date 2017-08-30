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

	DropStats = CreateDefaultSubobject<UItemDropStat>(TEXT("DropComponent"));
	this->AddOwnedComponent(DropStats);

	PerceptionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PerceptionSphere"));
	PerceptionSphere->SetupAttachment(RootComponent);
	PerceptionSphere->SetSphereRadius(500.0f);

	PerceptionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::DetectPlayer);
	PerceptionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::UnDetectPlayer);

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageArea"));
	DamageBox->SetupAttachment(RootComponent);

	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::DetectAttack);
	DamageBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::UnDetectAttack);

	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::DetectDamage);

	DyingTime = 5.0f;
	AttackInterval = 3.0f;
	bIsDying = false;
	
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

void AEnemyCharacter::Death() {
	Destroy();
}

void AEnemyCharacter::DropItems() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Drop Stuff")));
	if (DropStats->GetDrop()) {
		int32 quant = DropStats->GetDropQuantity();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Drop Quant %d"),quant));
		for (int32 i = 0; i < quant; i++) {
			FVector Origin = GetActorLocation() + FVector(0.0f,0.0f,50.0f);
			FVector Extent(50.0f, 50.0f, 50.0f);
			FVector Result = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
			//Origin += Result + FVector(0.0f, 0.0f, 100.0f);
			FRotator Rot = FRotationMatrix::MakeFromX(Result).Rotator();
			FTransform Trans(Rot,Result);
			FItem ite = DropStats->GetDropItem();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Droping..%f,%f,%f"),Origin.X,Origin.Y,Origin.Z));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Drop =%s"), *FItem::GetItemEnumAsString(ite.type)));
			DropItem(Trans,ite);
		}
	}
	DropStats->DropPoints();
}

void AEnemyCharacter::DetectDeath() {
	if (LifeStats->IsDead() && !bIsDying) {
		bIsDying = true;
		DropItems();
		GetKilled();
		GetWorld()->GetTimerManager().SetTimer(DeadTimer, this, &AEnemyCharacter::Death, DyingTime, false);
	}
}

void AEnemyCharacter::DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LifeStats->DetectDamage(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UpdateHUDLife();
	DetectDeath();
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
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Detected %d"), PlayerActors.Num()));
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

void AEnemyCharacter::DoDamage() {
	if (ProjectileClass != nullptr && !bIsDying) {
		if (PlayerActorsAttack.Num() > 0) {
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.Instigator = Instigator;
			FVector loc = PlayerActorsAttack.CreateConstIterator()->Value->GetActorLocation();
			loc.Z += 60.0f;
			FTransform trans(loc);
			GetWorld()->SpawnActor<AProjectile>(ProjectileClass, trans, SpawnInfo);
		}
	}
}

void AEnemyCharacter::DetectAttack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASurvivalWaveCharacter* play = Cast<ASurvivalWaveCharacter>(OtherActor);
	if (play != nullptr) {
		int64 add = (int64)(OtherActor);
		int64 poi = (int64)(&OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player >>Detect %s %llx %llX"),*play->GetName(),poi,add));
		if (!PlayerActorsAttack.Contains(add))
			PlayerActorsAttack.Add(add, OtherActor);
		GetWorld()->GetTimerManager().SetTimer(AttackTimer,this, &AEnemyCharacter::DoDamage,AttackInterval,true);
	}
}

void AEnemyCharacter::UnDetectAttack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	ASurvivalWaveCharacter* play = Cast<ASurvivalWaveCharacter>(OtherActor);
	if (play != nullptr) {
		int64 add = (int64)(OtherActor);
		int64 poi = (int64)(&OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Detect>> %s %llx %llX"), *play->GetName(), poi, add));
		if (PlayerActorsAttack.Contains(add))
			PlayerActorsAttack.Remove(add);
		if(PlayerActorsAttack.Num() <= 0)
			GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	}
}

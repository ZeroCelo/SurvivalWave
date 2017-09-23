// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "Projectile.h"
#include "Weapon_Projectile.h"
#include "SurvivalWaveCharacter.h"
#include "EnemyCharacter.h"
#include "AISimpleController.h"

//#include "EngineGlobals.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(50.0f);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::ProjectileHit);

	FXPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FXPoint"));
	FXPoint->SetupAttachment(RootComponent);

	//BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	
	ProjectileMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoving"));
	this->AddOwnedComponent(ProjectileMove);
	
	DamageStats = CreateDefaultSubobject<UDamageStat>(TEXT("DamageComponent"));
	this->AddOwnedComponent(DamageStats);

	AreaDamageTime = 0.050f;
	bAreaDamage = false;

	//ProjectileMove->SetUpdatedComponent(RootComponent);

	//WeaponParent = nullptr;
	//Damage = 1.0f;
	//MaxDistance = 1000.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (ProjectileFX != nullptr) {
		//if(FXPoint == nullptr)
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ImpactNull")));
		//ProjectilePSC = UGameplayStatics::SpawnEmitterAttached(ProjectileFX, RootComponent);
		ProjectilePSC = UGameplayStatics::SpawnEmitterAttached(ProjectileFX, FXPoint);
	}
	StartPosition = GetActorLocation();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector dist = GetActorLocation() - StartPosition;
	float d = dist.Size();
	if (d > DamageStats->GetDamageRange()) {
		Impact(GetActorLocation(),GetActorRotation());
	}
}

bool AProjectile::DoesAreaDamage() {
	return bAreaDamage;
}

void AProjectile::LateDestroy() {
	Destroy();
}

void AProjectile::Impact(FVector location, FRotator rotation) {
	if (ImpactFX != nullptr) {
		//UParticleSystemComponent* shot = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TraceFX,FirePoint->GetComponentLocation(),FirePoint->GetComponentRotation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, location, rotation);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ImpactTime %f"), GetWorld()->GetTimeSeconds()));
	if (!bAreaDamage) {
		if (AreaProjectileClass != nullptr) {
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = GetOwner();
			SpawnInfo.Instigator = Instigator;
			//AProjectile* proj = 
			//GetWorld()->SpawnActor<AProjectile>(AreaProjectileClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
			GetWorld()->SpawnActor<AProjectile>(AreaProjectileClass, location, rotation, SpawnInfo);
		}
		Destroy();
	}
	else {
		//bDestroying = true;
		//if (!bDestroying) {
			GetWorld()->GetTimerManager().SetTimer(AreaTimer, this, &AProjectile::LateDestroy, AreaDamageTime, false);
		//}
	}
}

void AProjectile::ProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor != GetOwner()) {
		Impact(CollisionSphere->GetComponentLocation(), CollisionSphere->GetComponentRotation());
		if (bAreaDamage) {
			ACharacter* cha = Cast<ACharacter>(OtherActor);
			if (ImpactFX != nullptr && cha != nullptr) {
				//UParticleSystemComponent* shot = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TraceFX,FirePoint->GetComponentLocation(),FirePoint->GetComponentRotation());
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, OtherActor->GetActorLocation());
				AAISimpleController* con = Cast<AAISimpleController>(cha->GetController());
				if (con != nullptr) {
					con->SuspiciousTarget(GetOwner());
				}
			}
		}
	}
}

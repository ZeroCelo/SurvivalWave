// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "Projectile.h"
#include "Weapon_Projectile.h"
#include "SurvivalWaveCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	FXPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FXPoint"));
	//BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoving"));

	RootComponent = CollisionSphere;
	FXPoint->SetupAttachment(RootComponent);

	CollisionSphere->SetSphereRadius(50.0f);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::ProjectileHit);

	//WeaponParent = nullptr;
	Damage = 1.0f;
	MaxDistance = 1000.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (ProjectileFX != nullptr) {
		ProjectilePSC = UGameplayStatics::SpawnEmitterAttached(ProjectileFX, FXPoint);
	}
	StartPosition = this->GetActorLocation();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector dist = this->GetActorLocation() - StartPosition;
	float d = dist.Size();
	if (d > MaxDistance) {
		Impact(GetActorLocation(),GetActorRotation());
		this->Destroy();
	}
}

void AProjectile::Impact(FVector location, FRotator rotation) {
	if (ImpactFX != nullptr) {
		//UParticleSystemComponent* shot = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TraceFX,FirePoint->GetComponentLocation(),FirePoint->GetComponentRotation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, location, rotation);
	}
	this->Destroy();
}

void AProjectile::ImpactCheck(AActor* Other) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("ImpactCheck")));
	ASurvivalWaveCharacter* Player = Cast<ASurvivalWaveCharacter>(Other);
	if (Player != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Impact Player!")));
				
		Player->CheckDamage(this);
	}
}

void AProjectile::ProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Impact(CollisionSphere->GetComponentLocation(), CollisionSphere->GetComponentRotation());
	ImpactCheck(OtherActor);
}

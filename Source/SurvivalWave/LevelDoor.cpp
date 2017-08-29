// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "LevelDoor.h"

#include "SurvivalWaveCharacter.h"
#include "EngineGlobals.h"


// Sets default values
ALevelDoor::ALevelDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<UArrowComponent>(TEXT("Origin"));
	RootComponent = Origin;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxArea"));
	Box->SetupAttachment(RootComponent);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ALevelDoor::DetectPlayer);
	Box->OnComponentEndOverlap.AddDynamic(this, &ALevelDoor::UnDetectPlayer);
	//RootComponent = Box;
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(RootComponent);
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(RootComponent);
	Lock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock"));
	Lock->SetupAttachment(LeftDoor);
	Front = CreateDefaultSubobject<UArrowComponent>(TEXT("Front"));
	Front->SetupAttachment(RootComponent);
		
	PlayerNear = 0;
	bActive = true;
	bOpenBothSides = false;
}

void ALevelDoor::OnConstruction(const FTransform& Transform){
	SetupActive();
}

// Called when the game starts or when spawned
void ALevelDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALevelDoor::IsDoorActive() {
	return bActive;
}

void ALevelDoor::SetDoorActive(bool bnew) {
	bActive = bnew;
	SetupActive();
}

void ALevelDoor::CheckPlayer(AActor* Other) {
	
		
	if (PlayerNear <= 0) {
		CloseDoor();
		ASurvivalWaveCharacter* Play = Cast<ASurvivalWaveCharacter>(Other);
		if (Play != nullptr && !bOpenBothSides) {
			FVector Forward = GetActorForwardVector();
			FVector PlayerDir = GetActorLocation() - Play->GetActorLocation();
			PlayerDir.Normalize();
			Forward -= PlayerDir;
			if (Forward.Size() > 1.45f) {
				bActive = false;
				SetupActive();
			}
		}
	}
	else {
		OpenDoor();
	}
}

void ALevelDoor::DetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ASurvivalWaveCharacter* Play = Cast<ASurvivalWaveCharacter>(OtherActor);
	
	if (Play != nullptr) {
		++PlayerNear;
		CheckPlayer(OtherActor);
	}
}

void ALevelDoor::UnDetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ASurvivalWaveCharacter* Play = Cast<ASurvivalWaveCharacter>(OtherActor);

	if (Play != nullptr) {
		--PlayerNear;
		CheckPlayer(OtherActor);
	}
}

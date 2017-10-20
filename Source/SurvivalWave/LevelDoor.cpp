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
	NextRoomIndex = -1;
	bActive = true;
	bDoorClosed = true;
	bLevelLoaded = false;
	bLevelShouldUnload = false;
	bWaitLevel = true;
	bOpenBothSides = true;
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

bool ALevelDoor::IsDoorWaiting() {
	return bWaitLevel && !bLevelLoaded && PlayerNear > 0;
}

bool ALevelDoor::IsDoorLevelLoaded() {
	return bLevelLoaded;
}

bool ALevelDoor::IsDoorClosed() {
	return bDoorClosed;
}

bool ALevelDoor::IsDoorActive() {
	return bActive;
}

bool ALevelDoor::ShouldLevelUnload() {
	return bLevelShouldUnload;
}

FString ALevelDoor::GetLevelName() {
	return LevelStreamName;
}

int32 ALevelDoor::GetNextRoomIndex() {
	return NextRoomIndex;
}

void ALevelDoor::SetDoorActive(bool bnew) {
	bActive = bnew;
	SetupActive();
}

void ALevelDoor::SetLevelLoaded(bool bnew) {
	bLevelLoaded = bnew;
	SetupActive();
	CheckPlayer(nullptr);
}

void ALevelDoor::SetLevelUnload(bool bnew) {
	bLevelShouldUnload = bnew;
}

void ALevelDoor::SetWaitLevel(bool bnew) {
	bWaitLevel = bnew;
	
	SetupActive();
}

float ALevelDoor::CheckActorDirection(AActor* Other) {
	ASurvivalWaveCharacter* Play = Cast<ASurvivalWaveCharacter>(Other);
	if (Play != nullptr) {
		FVector Forward = GetActorForwardVector();
		FVector PlayerDir = GetActorLocation() - Play->GetActorLocation();
		PlayerDir.Normalize();
		Forward -= PlayerDir;
		return Forward.Size();
	}
	return -1.0f;
}

void ALevelDoor::CheckPlayer(AActor* Other) {
	
	if (PlayerNear <= 0) {
		CloseDoor();
		float dir = CheckActorDirection(Other);
		if (dir > 1.45f && !bOpenBothSides) {
			bActive = false;
			SetupActive();
		}
		if (dir >= 0.0f && dir <= 1.45f) {
			bLevelShouldUnload = true;
		}
		//else if(dir > -1.0f){
		else {
			bLevelShouldUnload = false;
		}
	}
	else {
		if (!bWaitLevel) {
			OpenDoor();
		}
		else {
			if (!IsDoorWaiting()) {
				OpenDoor();
			}
			else
				SetupActive();
		}
	}
}

void ALevelDoor::DetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	ASurvivalWaveCharacter* Play = Cast<ASurvivalWaveCharacter>(OtherActor);
	if (Play != nullptr) {
		++PlayerNear;
		if (bActive) {
			CheckPlayer(OtherActor);
		}
	}
}

void ALevelDoor::UnDetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	
	ASurvivalWaveCharacter* Play = Cast<ASurvivalWaveCharacter>(OtherActor);
	if (Play != nullptr) {
		--PlayerNear;
		if (bActive) {
			CheckPlayer(OtherActor);
		}
	}
}

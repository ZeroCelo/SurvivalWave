// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "KeySwitch.h"
#include "KeyItemInventory.h"
#include "SurvivalWaveGameMode.h"

#include "EngineGlobals.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AKeySwitch::AKeySwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	RootComponent = SwitchMesh;

	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	//CollisionBox->SetupAttachment(RootComponent);

	//CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AKeySwitch::DoorCheck);

	MsgKeyNeed = MsgKeyNeed.FromString("Need Key");
	MsgKeyFound = MsgKeyNeed.FromString("Door Open");

	bDisplayMsg = true;
	bDisplayKey = false;
	bDoorOpen = false;
	KeyValue = 0.0f;
	FindDoorTime = 0.7f;
	Door = nullptr;
}

// Called when the game starts or when spawned
void AKeySwitch::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(FindTimer, this, &AKeySwitch::FindDoor, FindDoorTime, false);

	//TArray<AActor*> FoundSpawners;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), FoundSpawners);
}

// Called every frame
void AKeySwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeySwitch::FindDoor() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Finding Door")));
	TArray<AActor*> FoundDoors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelDoor::StaticClass(), FoundDoors);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Door %d"), FoundDoors.Num()));
	for (auto Actor : FoundDoors) {
		ALevelDoor* DoorActor = Cast<ALevelDoor>(Actor);
		if (DoorActor != nullptr) {
			if (DoorActor->GetName().Contains(DoorName)) {
				Door = DoorActor;
				Door->Box->OnComponentBeginOverlap.AddDynamic(this, &AKeySwitch::DoorCheck);
				break;
			}
		}
	}
	CheckDoor();
	//if(Door != nullptr)
		//Door->SetDoorActive(false);
}

void AKeySwitch::CheckDoor() {
	if (Door != nullptr) {
		//Door->CloseDoor();
		//Door->SetDoorActive(false);
		//Door->SetupDoorKey(KeyValue, 1.0f, 1.0f);
		//if (Door->IsDoorClosed()) {
		
		if (!bDoorOpen) {
			Door->SetDoorActive(false);
			Door->SetupDoorKey(KeyValue,1.0f,1.0f);
		}
		else if(Door->IsDoorClosed()){
			Door->SetDoorActive(true);
			//Door->SetupDoorKey(KeyValue, 1.0f, 1.0f);
		}
		//else if(!bDoorOpen){
			//Door->CloseDoor();
			//Door->SetupDoorKey(KeyValue, 1.0f, 1.0f);
			//Door->SetDoorActive(true);
		//}

	}
}

void AKeySwitch::PrintMessage(bool bHasKey) {
	if (bDisplayMsg) {
		ASurvivalWaveGameMode* GM = Cast<ASurvivalWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM != nullptr) {
			if (GM->GetCurrentState() == EWaveState::EWaveClear || GM->GetCurrentState() == EWaveState::ELevelFinish) {
				if (bHasKey)
					GM->UpdateInfo(MsgKeyFound.ToString());
				else if (!bDoorOpen) {
					FString msg = MsgKeyNeed.ToString();
					if (bDisplayKey) {
						int32 key = (int32)KeyValue;
						FString KeyStr; KeyStr.AppendInt(key);
						msg += ' '; msg += KeyStr;
					}
					GM->UpdateInfo(msg);
				}
			}
		}
	}
}

void AKeySwitch::DoorCheck(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Checking Door")));
	
	UKeyItemInventory* Keys = OtherActor->FindComponentByClass<UKeyItemInventory>();

	if (Keys != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Keys Detect")));
		if (Keys->HasKey(KeyValue)) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Key!!")));
			//CheckDoor();
			if (Door != nullptr) {
				bDoorOpen = true;
				ASurvivalWaveGameMode* GM = Cast<ASurvivalWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GM != nullptr) {
					//if (GM->GetCurrentState() == EWaveState::EWaveClear || GM->GetCurrentState() == EWaveState::ELevelFinish || GM->GetCurrentState() == EWaveState::EUnknown) {
					if (!(GM->GetCurrentState() == EWaveState::EWaveWaiting) && !(GM->GetCurrentState() == EWaveState::EGameOver) && !(GM->GetCurrentState() == EWaveState::EWaveBegin)) {
						if (!Door->IsDoorActive()) {
							PrintMessage(true);
							Door->SetDoorActive(true);

							if(!Door->IsDoorWaiting())
								Door->OpenDoor();						
						}
					}
					else {
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Checking...")));
						Door->SetDoorActive(false);
						Door->SetupDoorKey(KeyValue, 1.0f, 1.0f);
						//CheckDoor();
					}
				}
				else {
					if (!Door->IsDoorActive()) {
						PrintMessage(true);
						Door->SetDoorActive(true);
						
						if (!Door->IsDoorWaiting())
							Door->OpenDoor();
					}
				}
			}
		}
		else {
			if (Door != nullptr) {
				if (!Door->IsDoorActive()) {
					PrintMessage(false);
				}
			}
		}
	}
}

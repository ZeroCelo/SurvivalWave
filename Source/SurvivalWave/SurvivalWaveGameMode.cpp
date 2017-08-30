// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SurvivalWave.h"
#include "SurvivalWaveGameMode.h"
#include "SurvivalWaveCharacter.h"
#include "InfoHUDClass.h"
#include "EnemyCharacter.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "EngineGlobals.h"


ASurvivalWaveGameMode::ASurvivalWaveGameMode()
{
	
	CurrentWave = 0;
	MaxWave = 2;
	CheckTime = 0.10f;
	PreWaveTime = 2.0f;
	LevelEndTime = 6.0f;
	WaveTimeCount = 0.0f;
	WaveTimeStart = 3.0f;
	Score = 0;
	StartState = EWaveState::ELobby;
	MsgLobby = MsgLobby.FromString("Game Start");
	MsgWaveStart = MsgWaveStart.FromString("Fight to Survive");
	MsgWaveBegin = MsgWaveBegin.FromString("Ready");
	MsgLevelFinish = MsgLevelFinish.FromString("Legends Never Die");
	MsgLevelEnd = MsgLevelEnd.FromString("GG WP");
	MsgWavePrefix = MsgWavePrefix.FromString("Wave ");
	MsgWaveSuffix = MsgWaveSuffix.FromString(" End");
	LastDoorName = "WaveEndDoor";
	LobbyDoorName = "WaveLobbyDoor";
	LobbyDoor = nullptr;
	LastDoor = nullptr;
}

void ASurvivalWaveGameMode::BeginPlay(){
	Super::BeginPlay();

	//Get Spawners
	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), FoundSpawners);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Spawn %d"),FoundSpawners.Num()));
	for (auto Actor : FoundSpawners) {
		ASpawner* SpawnVolumeActor = Cast<ASpawner>(Actor);
		if (SpawnVolumeActor != nullptr) {
			if (SpawnVolumeActor->GetName().Contains("SpawnerLevel")) {
				SpawnEnemy.AddUnique(SpawnVolumeActor);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Found Spawn %s"), *SpawnVolumeActor->GetName()));
			}
			else if (SpawnVolumeActor->GetName().Contains("SpawnerCoin")) {
				AItemSpawner* ite = Cast<AItemSpawner>(Actor);
				SpawnLoot.AddUnique(ite);
			}
		}
	}
	SpawnEnemy.Sort([](const ASpawner& A, const ASpawner& B) {
		return A.GetName() < B.GetName();
	});
	//Get Doors
	TArray<AActor*> FoundDoors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelDoor::StaticClass(), FoundDoors);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Door %d"), FoundDoors.Num()));
	for (auto Actor : FoundDoors) {
		ALevelDoor* DoorActor = Cast<ALevelDoor>(Actor);
		if (DoorActor != nullptr) {
			if (DoorActor->GetName().Contains(LobbyDoorName)) {
				LobbyDoor = DoorActor;
			}
			else if (DoorActor->GetName().Contains(LastDoorName)) {
				LastDoor = DoorActor;
			}
			else if (DoorActor->GetName().Contains("WaveDoor")) {
				LevelDoors.AddUnique(DoorActor);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Found Door %s"), *DoorActor->GetName()));
			}
		}
	}
	LevelDoors.Sort([](const ALevelDoor& A, const ALevelDoor& B) {
		return A.GetName() < B.GetName();
	});

	//Setup the HUD
	if (InfoHUDClass != nullptr) {
		InfoHUD = CreateWidget<UUserWidget>(GetWorld(), InfoHUDClass);
		if (InfoHUD != nullptr) InfoHUD->AddToViewport();
	}

	SetCurrentState(StartState);
}

EWaveState ASurvivalWaveGameMode::GetCurrentState() const {
	return CurrentState;
}

int32 ASurvivalWaveGameMode::GetCurrentScore() const {
	return Score;
}

void ASurvivalWaveGameMode::AddScore(int32 point){
	Score += point;
}

void ASurvivalWaveGameMode::SetCurrentState(EWaveState NewState) {
	//Set current State
	CurrentState = NewState;
	//handle the new current state
	HandleNewState(CurrentState);
}

void ASurvivalWaveGameMode::HandleNewState(EWaveState NewState) {
	switch (NewState) {
	//We are waiting for the game to start
	case EWaveState::ELobby:
	{
		Lobby();
	}
	break;
	//If the game is playing
	case EWaveState::EWaveBegin:
	{
		BeginWave();
		//Spawn volumes active
		//for (ASpawnVolume* Volume : SpawnVolumeActors) {
			//Volume->SetSpawningActive(true);
		//}
	}
	break;
	//If we won
	case EWaveState::ELevelFinish:
	{
		LevelFinish();
		//Spawn volumes inactive
		//for (ASpawnVolume* Volume : SpawnVolumeActors) {
			//Volume->SetSpawningActive(false);
		//}
	}
	break;
	//If we lost
	case EWaveState::EGameOver:
	{
		GameOver();
		//Spanw volumes inactive
		//for (ASpawnVolume* Volume : SpawnVolumeActors) {
			//Volume->SetSpawningActive(false);
		//}
		//Block player input
		/*APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController) {
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}*/
	}
	break;
	//If Wave Ended wait for player to go to next area
	case EWaveState::EWaiting:
	{
		BeginWait();
	}
	break;
	//Unkown/default state
	default:
	case EWaveState::EUnknown:
	{
		//do nothing
		UpdateInfo("Unknown State");
	}
	break;
	}
}

void ASurvivalWaveGameMode::UpdateInfo(FString Info) {
	if (InfoHUD != nullptr) {
		UInfoHUDClass* InfoWidget = Cast<UInfoHUDClass>(InfoHUD);
		if (InfoWidget != nullptr)
			InfoWidget->SetInfo(Info);
	}
}

void ASurvivalWaveGameMode::LobbyWait() {
	if (LobbyDoor != nullptr) {
		if (!LobbyDoor->IsDoorActive()) {
			SetCurrentState(EWaveState::EWaveBegin);
		}
	}
}

void ASurvivalWaveGameMode::Lobby() {
	UpdateInfo(MsgLobby.ToString());
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::LobbyWait, CheckTime, true);
}

void ASurvivalWaveGameMode::PreWaveStart() {
	if (WaveTimeCount < WaveTimeStart) {
		int32 Time = WaveTimeStart - WaveTimeCount;
		FString info; info.AppendInt(Time);
		UpdateInfo(info);
		GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::PreWaveStart, 1.0f, false);
		WaveTimeCount += 1.0f;
	}
	else {
		WaveTimeCount = 0.0f;
		WaveStart();
	}
}

void ASurvivalWaveGameMode::WaveStart() {
	UpdateInfo(MsgWaveStart.ToString());
	SpawnEnemy[CurrentWave]->SetSpawningActive(true);
	SetCurrentState(EWaveState::EWaiting);
}

void ASurvivalWaveGameMode::WaveWait() {
	if (LevelDoors.Num()) {
		if (!LevelDoors[CurrentWave - 1]->IsDoorActive()) {
			SetCurrentState(EWaveState::EWaveBegin);
		}
	}
}

void ASurvivalWaveGameMode::BeginWave() {
	GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::PreWaveStart, PreWaveTime, false);
	UpdateInfo(MsgWaveBegin.ToString());
}

void ASurvivalWaveGameMode::LevelFinish() {
	UpdateInfo(MsgLevelFinish.ToString());
	GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::CheckFinish, CheckTime, true);
}

void ASurvivalWaveGameMode::CheckFinish() {
	if (LastDoor != nullptr) {
		if (!LastDoor->IsDoorActive()) {
			UpdateInfo(MsgLevelEnd.ToString());
			for (int i = 0; i < SpawnLoot.Num(); i++) {
				SpawnLoot[i]->SpawnPoints();
			}
			GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::GameEnd, LevelEndTime, false);
		}
	}
}

void ASurvivalWaveGameMode::GameOver() {

}

void ASurvivalWaveGameMode::BeginWait() {
	GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::WaveWaiting, CheckTime, true);
}

void ASurvivalWaveGameMode::WaveWaiting() {
	bool bWaveDone = false;
	bool bPlayerDead = false;
	if (!SpawnEnemy[CurrentWave]->IsActive()) {
		TArray<AActor*> FoundEnemy;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundEnemy);
		bWaveDone = (FoundEnemy.Num() <= 0);
	}

	if (bPlayerDead) {

	}
	else if (bWaveDone) {
		FString info(MsgWavePrefix.ToString());
		CurrentWave++;
		info.AppendInt(CurrentWave);
		info += MsgWaveSuffix.ToString();
		UpdateInfo(info);
		if (CurrentWave < MaxWave) {
			GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::WaveWait, CheckTime, true);
			LevelDoors[CurrentWave - 1]->SetDoorActive(true);
		}
		else {
			GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			LastDoor->SetDoorActive(true);
			SetCurrentState(EWaveState::ELevelFinish);
		}
	}
}

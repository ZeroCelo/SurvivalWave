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
	
	WaveComplete = 0;
	MaxWaves = 1;
	CheckTime = 0.10f;
	CheckDoorTime = 0.250f;
	LastRoomLoad = true;// .Add(-1);
	LoadedRooms.Add(0);
	PreWaveTime = 2.0f;
	LevelEndTime = 6.0f;
	LevelGameOverTime = 8.0f;
	WaveTimeCount = 0.0f;
	WaveTimeStart = 3.0f;
	Score = 0;
	StartState = EWaveState::ELobby;
	MsgLoot = MsgLobby.FromString("Loot Room");
	MsgLobby = MsgLobby.FromString("Game Start");
	MsgWaveStart = MsgWaveStart.FromString("Fight to Survive");
	MsgWaveBegin = MsgWaveBegin.FromString("Ready");
	MsgLevelFinish = MsgLevelFinish.FromString("Legends Never Die");
	MsgLevelEnd = MsgLevelEnd.FromString("GG WP");
	MsgGameOver = MsgLevelEnd.FromString("GG EZ");
	MsgWavePrefix = MsgWavePrefix.FromString("Wave ");
	MsgWaveSuffix = MsgWaveSuffix.FromString(" End");
	LobbyRoomIndex = 0;
	LastRoomIndex = -1;
}

void ASurvivalWaveGameMode::BeginPlay(){
	Super::BeginPlay();

	CallCheckDoors(true);
			
	//Setup the HUD
	if (InfoHUDClass != nullptr) {
		InfoHUD = CreateWidget<UUserWidget>(GetWorld(), InfoHUDClass);
		if (InfoHUD != nullptr) InfoHUD->AddToViewport();
	}

	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::CallStartState, 0.50f, false);
	
}

void ASurvivalWaveGameMode::CallStartState() {
	SetCurrentState(StartState);
}

void ASurvivalWaveGameMode::FindSpawners() {
	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), FoundSpawners);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Spawn %d"),FoundSpawners.Num()));
	for (auto Actor : FoundSpawners) {
		ASpawner* SpawnVolumeActor = Cast<ASpawner>(Actor);
		if (SpawnVolumeActor != nullptr) {
			for (auto& Room: WaveRooms) {
				if (Room.AddEnemySpawn(SpawnVolumeActor))
					break;
				if (Room.AddLootSpawn(SpawnVolumeActor))
					break;
			}
		}
	}
}

void ASurvivalWaveGameMode::FindDoors() {
	TArray<AActor*> FoundDoors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelDoor::StaticClass(), FoundDoors);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found Door %d"), FoundDoors.Num()));
	for (auto Actor : FoundDoors) {
		ALevelDoor* DoorActor = Cast<ALevelDoor>(Actor);
		if (DoorActor != nullptr) {
			for (auto& Room : WaveRooms) {
				if (Room.AddDoor(DoorActor))
					break;
			}
		}
	}
}

void ASurvivalWaveGameMode::CallCheckDoors(bool Yes) {
	if (Yes) {
		GetWorld()->GetTimerManager().SetTimer(CheckDoorTimer, this, &ASurvivalWaveGameMode::CheckDoors, CheckDoorTime, true);
	}
	else {
		GetWorld()->GetTimerManager().ClearTimer(CheckDoorTimer);
	}
}

void ASurvivalWaveGameMode::CheckDoors() {
	//Check if level was loaded before and updates its spawners/doors
	if (LastRoomLoad) {
		for (auto &Room : WaveRooms) {
			Room.LevelDoors.Empty();
			Room.SpawnEnemy.Empty();
			Room.SpawnLoot.Empty();
		}
		FindSpawners();
		FindDoors();
		for (auto& Room : WaveRooms) {
			if (Room.IsWaveDone()) {
				Room.DoorsStateLoad();
			}
			for (auto &RoomDoor : Room.LevelDoors) {
				bool DoorHasLoaded = RoomDoor->IsDoorLevelLoaded() || LoadedRooms.Find(RoomDoor->NextRoomIndex) != INDEX_NONE;
				if (DoorHasLoaded)
					RoomDoor->SetLevelLoaded(true);
			}
		}
		LastRoomLoad = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Last Load Rooms %d"), LoadedRooms.Num()));
	}
	else {
		TArray<int32> RemovedRooms;
		//Check Wich Room can be unloaded
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Loaded Rooms %d"), LoadedRooms.Num()));
		for (auto &Index : LoadedRooms) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("LoadedRooms[%d] Doors %d"), Index, WaveRooms[Index].LevelDoors.Num()));
			if (Index >= 0 && Index < WaveRooms.Num()) {
				for (auto &RoomDoor : WaveRooms[Index].LevelDoors) {
					if (RoomDoor != nullptr) {
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("   Rooms Door %s"), *RoomDoor->LevelStreamName));
						if (RoomDoor->ShouldLevelUnload() && RoomDoor->IsDoorLevelLoaded() && RoomDoor->IsDoorClosed()) {
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("   Rooms Door Unload %s"), *RoomDoor->LevelStreamName));
							RoomDoor->SetLevelLoaded(false);
							RoomDoor->SetLevelUnload(false);
							if (WaveRooms[RoomDoor->NextRoomIndex].IsWaveDone() && RoomDoor->NextRoomIndex != LastRoomIndex && RoomDoor->NextRoomIndex != LobbyRoomIndex)
								WaveRooms[RoomDoor->NextRoomIndex].DoorsStateSave();
							UnLoadLevel(0, RoomDoor->LevelStreamName);
							RemovedRooms.AddUnique(RoomDoor->NextRoomIndex);
							LastRoomLoad = true;
						}
					}
				}
			}
		}
		for (auto &Ind : RemovedRooms) {
			LoadedRooms.Remove(Ind);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Removed Rooms %d"), RemovedRooms.Num()));

		//Check wich door needs loading
		for (auto &Index : LoadedRooms) {
			if (Index >= 0 && Index < WaveRooms.Num()) {
				for (auto &RoomDoor : WaveRooms[Index].LevelDoors) {
					if (RoomDoor != nullptr) {
						if (RoomDoor->IsDoorActive()) {
							bool DoorHasLoaded = RoomDoor->IsDoorLevelLoaded() || LoadedRooms.Find(RoomDoor->NextRoomIndex) != INDEX_NONE;
							bool DoorNewLoad = LoadedRooms.Num() < 2 && !DoorHasLoaded;

							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Room Waiting %d CanLoad %d LoadedRooms %d"), RoomDoor->IsDoorWaiting(),DoorHasLoaded, LoadedRooms.Num()< 2));
							if (RoomDoor->IsDoorWaiting() && (DoorNewLoad || DoorHasLoaded) && (WaveRooms[Index].IsWaveDone() || DoorHasLoaded || CurrentState == EWaveState::EUnknown)) {
								//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Room Waiting %s"), *RoomDoor->LevelStreamName));
								LoadLevel(0, RoomDoor->LevelStreamName);

								RoomDoor->SetLevelLoaded(true);
								RoomDoor->SetLevelUnload(false);
								LoadedRooms.AddUnique(RoomDoor->NextRoomIndex);
								LastRoomLoad = true;
							}
						}
					}
				}
			}
		}
	}
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
	//Setup Wave to start
	case EWaveState::EWaveBegin:
	{
		BeginWave();
	}
	break;
	//Level Won
	case EWaveState::ELevelFinish:
	{
		LevelFinish();
	}
	break;
	//Player Died
	case EWaveState::EGameOver:
	{
		GameOver();
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

void ASurvivalWaveGameMode::UpdateError(FString Info) {
	if (InfoHUD != nullptr) {
		UInfoHUDClass* InfoWidget = Cast<UInfoHUDClass>(InfoHUD);
		if (InfoWidget != nullptr)
			InfoWidget->SetError(Info);
	}
}

void ASurvivalWaveGameMode::LobbyWait() {
	if (LoadedRooms.Num()) {
		if (LoadedRooms[0] != LobbyRoomIndex) {
			
			if(!WaveRooms[LoadedRooms[0]].IsWaveDone()){
				WaveRooms[LoadedRooms[0]].SetDoorsActive(false);
				SetCurrentState(EWaveState::EWaveBegin);
			}
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
	if (LoadedRooms.Num()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Wave Start %d"), LoadedRooms[0]));
		WaveRooms[LoadedRooms[0]].SpawnEnemies();
		SetCurrentState(EWaveState::EWaiting);
	}
}

void ASurvivalWaveGameMode::WaveWait() {
	
	if (LoadedRooms.Num()) {
		if (LoadedRooms[0] == LastRoomIndex) {
			//UpdateInfo(MsgLevelEnd.ToString());
			//WaveRooms[LoadedRooms[0]].SetDoorsActive(false);
			//WaveRooms[LoadedRooms[0]].SpawnLoots();

			//GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			//GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::CallGameEnd, LevelEndTime, false);
			GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			WaveRooms[LoadedRooms[0]].SetDoorsLock();
			SetCurrentState(EWaveState::ELevelFinish);
		}
		else if (!WaveRooms[LoadedRooms[0]].HaveEnemy() && !WaveRooms[LoadedRooms[0]].IsWaveDone()) {
			UpdateInfo(MsgLoot.ToString());
			
			WaveRooms[LoadedRooms[0]].SetWaveComplete(true);
			WaveRooms[LoadedRooms[0]].SpawnLoots();
		}
		else if (!WaveRooms[LoadedRooms[0]].IsWaveDone()) {
			WaveRooms[LoadedRooms[0]].SetDoorsActive(false);
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

void ASurvivalWaveGameMode::CallGameEnd() {
	GameEnd();
}

void ASurvivalWaveGameMode::CheckFinish() {
	
	if (LoadedRooms.Num()) {
		if (LoadedRooms[0] == LastRoomIndex) {
			UpdateInfo(MsgLevelEnd.ToString());
			WaveRooms[LoadedRooms[0]].SetDoorsActive(false);
			WaveRooms[LoadedRooms[0]].SpawnLoots();

			GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::CallGameEnd, LevelEndTime, false);
		}
		else if (!WaveRooms[LoadedRooms[0]].HaveEnemy() && !WaveRooms[LoadedRooms[0]].IsWaveDone()) {
			UpdateInfo(MsgLoot.ToString());

			WaveRooms[LoadedRooms[0]].SetWaveComplete(true);
			WaveRooms[LoadedRooms[0]].SpawnLoots();
		}
	}
}

void ASurvivalWaveGameMode::GameOver() {
	UpdateInfo(MsgGameOver.ToString());

	GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::CallGameEnd, LevelGameOverTime, false);
	
	//Block player input
	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ASurvivalWaveCharacter* play = Cast<ASurvivalWaveCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (play != nullptr) {
		//PlayerController->SetCinematicMode(true, false, false, true, true);
		APlayerController* PlayerController = Cast<APlayerController>(play->GetController());
		play->DisableInput(PlayerController);
	}
	
	DeathAnim();

	
}

void ASurvivalWaveGameMode::BeginWait() {
	GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::WaveWaiting, CheckTime, true);
}

void ASurvivalWaveGameMode::WaveWaiting() {
	bool bWaveDone = false;
	bool bPlayerDead = false;

	if (LoadedRooms.Num()) {
		if (!WaveRooms[LoadedRooms[0]].IsEnemyActive()) {
			TArray<AActor*> FoundEnemy;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundEnemy);
			bWaveDone = (FoundEnemy.Num() <= 0);
		}
	}

	ASurvivalWaveCharacter* play = Cast<ASurvivalWaveCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (play != nullptr)
		bPlayerDead = play->LifeStats->IsDead();

	if (bPlayerDead) {
		SetCurrentState(EWaveState::EGameOver);
	}
	else if (bWaveDone) {
		FString info(MsgWavePrefix.ToString());
		++WaveComplete;
		info.AppendInt(WaveComplete);
		info += MsgWaveSuffix.ToString();
		UpdateInfo(info);

		WaveRooms[LoadedRooms[0]].SetDoorsLock();
		WaveRooms[LoadedRooms[0]].SetWaveComplete(true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Wave Done %d/%d"), WaveComplete,MaxWaves));


		if (WaveComplete < MaxWaves) {
			GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			GetWorld()->GetTimerManager().SetTimer(CheckTimer, this, &ASurvivalWaveGameMode::WaveWait, CheckTime, true);
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Wave Last %d"), LoadedRooms[0]));
			GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
			//WaveRooms[LoadedRooms[0]].SetDoorsActive(true);
			
			SetCurrentState(EWaveState::ELevelFinish);
		}
	}
}

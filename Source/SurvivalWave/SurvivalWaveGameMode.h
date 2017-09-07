// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "ItemSpawner.h"
#include "LevelDoor.h"

#include "SurvivalWaveGameMode.generated.h"

//If you want this to appear in BP, make sure to use this instead
USTRUCT(BlueprintType)
struct FLevelRoom
{
	GENERATED_BODY()

	//Always make USTRUCT variables into UPROPERTY()
	//any non-UPROPERTY() struct vars are not replicated
	//So to simplify your life for later debugging, always use UPROPERTY()
public:
	//If you want the property to appear in BP, make sure to use this instead
	//UPROPERTY(BlueprintReadOnly)
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPROPERTY(BlueprintReadOnly)
	TArray<class ASpawner*> SpawnEnemy;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPROPERTY(BlueprintReadOnly)
	TArray<class ALevelDoor*> LevelDoors;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPROPERTY(BlueprintReadOnly)
	TArray<class AItemSpawner*> SpawnLoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWaveDone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpawnEnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpawnLootName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelDoorName;

	//Index of Adjacent Rooms where the respective door should be set to (In)Active after wave completion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32,bool> LevelDoorActive;

	bool IsDoorsActive() {
		if (!LevelDoors.Num())
			return false;
		for (int i = 0; i < LevelDoors.Num(); ++i) {
			if (!LevelDoors[i]->IsDoorActive())
				return false;
		}
		return true;
	}

	bool HaveEnemy() {
		return (SpawnEnemy.Num() > 0);
	}
	
	bool IsEnemyActive() {
		for (int i = 0; i < SpawnEnemy.Num(); ++i) {
			if (SpawnEnemy[i]->IsActive())
				return true;
		}
		return false;
	}

	bool IsWaveDone() {
		return bWaveDone;
	}

	void SetWaveComplete(bool bnew) {
		bWaveDone = bnew;
	}

	void SetDoorsActive(bool bnew) {
		for (int i = 0; i < LevelDoors.Num(); ++i) {
			LevelDoors[i]->SetDoorActive(bnew);
		}
	}

	void SetDoorsLock() {
		for (auto& IndexElem : LevelDoorActive) {
			for (auto& RoomDoor : LevelDoors) {
				if(RoomDoor->NextRoomIndex == IndexElem.Key)
					RoomDoor->SetDoorActive(IndexElem.Value);
			}
		}
	}

	void DoorsStateSave() {
		for (auto& RoomDoor : LevelDoors) {
			LevelDoorActive.Add(RoomDoor->NextRoomIndex, RoomDoor->IsDoorActive());
		}
	}

	void DoorsStateLoad() {
		for (auto& RoomDoor : LevelDoors) {
			if (LevelDoorActive.Contains(RoomDoor->NextRoomIndex)) {
				RoomDoor->SetDoorActive(LevelDoorActive[RoomDoor->NextRoomIndex]);
			}
		}
	}

	void SpawnEnemies() {
		for (int i = 0; i < SpawnEnemy.Num(); ++i) {
			SpawnEnemy[i]->SetSpawningActive(true);
		}
	}

	void SpawnLoots() {
		for (int i = 0; i < SpawnLoot.Num(); ++i) {
			SpawnLoot[i]->SpawnPoints();
			SpawnLoot[i]->SetSpawningActive(true);
		}
	}

	bool AddEnemySpawn(ASpawner* Spa) {
		if (Spa != nullptr) {
			if (Spa->GetName().Contains(SpawnEnemyName)) {
				SpawnEnemy.AddUnique(Spa);
				return true;
			}
		}
		return false;
	}

	bool AddLootSpawn(ASpawner* Spa) {
		AItemSpawner* Loo = Cast<AItemSpawner>(Spa);
		if (Loo != nullptr) {
			if (Loo->GetName().Contains(SpawnLootName)) {
				SpawnLoot.AddUnique(Loo);
				return true;
			}
		}
		return false;
	}

	bool AddDoor(ALevelDoor* Doo) {
		if (Doo != nullptr) {
			if (Doo->GetName().Contains(LevelDoorName)) {
				LevelDoors.AddUnique(Doo);
				return true;
			}
		}
		return false;
	}

	//Constructor
	FLevelRoom() {
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		SpawnEnemyName = "Spawner";
		SpawnLootName = "SpawnerCoin";
		LevelDoorName = "WaveDoor";
		bWaveDone = false;
	}
};

//enum to store the current state of gameplay
UENUM(BluePrintType)
enum class EWaveState : uint8 {
	ELobby UMETA(DisplayName = "Lobby"),
	EWaiting UMETA(DisplayName = "Waiting"),
	EWaveBegin UMETA(DisplayName = "WaveBegin"),
	ELevelFinish UMETA(DisplayName = "LevelFinish"),
	EGameOver UMETA(DisplayName = "GameOver"),
	EUnknown UMETA(DisplayName = "Unknown")
};

UCLASS(minimalapi)
class ASurvivalWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	ASurvivalWaveGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> InfoHUDClass;

	UPROPERTY()
	class UUserWidget* InfoHUD;

	//Returns the current play state
	UFUNCTION(BlueprintPure, Category = "Power")
	EWaveState GetCurrentState() const;

	//Sets a new play state
	void SetCurrentState(EWaveState NewState);

	//Keeps track of game current state
	EWaveState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	EWaveState StartState;

	//Will handle any function calls that rely upon changing the playing state
	void HandleNewState(EWaveState NewState);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInfo")
	int32 WaveComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInfo")
	int32 MaxWaves;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInfo")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInfo")
	bool bAllPlayersDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float CheckTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float CheckDoorTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float PreWaveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float WaveTimeStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float LevelEndTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameLevels")
	int32 LobbyRoomIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameLevels")
	int32 LastRoomIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameLevels")
	TArray<FLevelRoom> WaveRooms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameLevels")
	bool LastRoomLoad;

	TArray<int32> LoadedRooms;

	float WaveTimeCount;

	FTimerHandle CheckTimer;
	FTimerHandle CheckDoorTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgLoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgLobby;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgWaveStart;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgWaveBegin;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgLevelFinish;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgLevelEnd;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgWavePrefix;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgWaveSuffix;	

	void UpdateInfo(FString Info);
	void UpdateError(FString Info);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentScore() const;

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 point);

	UFUNCTION(BlueprintCallable)
	void FindSpawners();

	UFUNCTION(BlueprintCallable)
	void FindDoors();

	void CheckDoors();
	void CallCheckDoors(bool Yes);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LoadLevel(int32 CurrentRoom, const FString& level);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UnLoadLevel(int32 CurrentRoom, const FString& level);

	void CallStartState();

	void Lobby();
	void LobbyWait();

	void BeginWave();
	void WaveWait();
	void PreWaveStart();
	void WaveStart();
	
	void LevelFinish();
	void CheckFinish();
	void CallGameEnd();
	void GameOver();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameEnd();
	//void GameEnd_Implementation();
	
	void BeginWait();
	void WaveWaiting();
	
	
};




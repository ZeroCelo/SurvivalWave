// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "ItemSpawner.h"
#include "LevelDoor.h"

#include "SurvivalWaveGameMode.generated.h"

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

	TArray<class ASpawner*> SpawnEnemy;
	TArray<class ALevelDoor*> LevelDoors;
	TArray<class AItemSpawner*> SpawnLoot;

	ALevelDoor* LobbyDoor;
	ALevelDoor* LastDoor;

	int32 CurrentWave;
	int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	int32 MaxWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float CheckTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float PreWaveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float WaveTimeStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig")
	float LevelEndTime;

	//Name of the Actor for the First Door the player faces
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FString LobbyDoorName;

	//Name of the Actor for the Last Door of the whole level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FString LastDoorName;

	float WaveTimeCount;

	FTimerHandle CheckTimer;

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

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentScore() const;

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 point);

	void Lobby();
	void LobbyWait();

	void BeginWave();
	void WaveWait();
	void PreWaveStart();
	void WaveStart();
	
	void LevelFinish();
	void CheckFinish();
	void GameOver();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameEnd();
	//void GameEnd_Implementation();
	
	void BeginWait();
	void WaveWaiting();
	
	
};




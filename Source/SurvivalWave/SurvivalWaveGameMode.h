// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "Spawner.h"
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

	float WaveTimeCount;

	FTimerHandle CheckTimer;

	void UpdateInfo(FString Info);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentScore() const;

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 point);

	void Lobby();
	void LobbyWait();

	void BeginWave();
	void PreWaveStart();
	void WaveStart();
	
	void LevelFinish();
	void GameOver();
	void GameEnd();
	
	void BeginWait();
	void WaveWaiting();
	
	
};




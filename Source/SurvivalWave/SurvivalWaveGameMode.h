// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "SurvivalWaveGameMode.generated.h"

UCLASS(minimalapi)
class ASurvivalWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	ASurvivalWaveGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> GameHUDClass;

	UPROPERTY()
	class UUserWidget* gameHUD;
};




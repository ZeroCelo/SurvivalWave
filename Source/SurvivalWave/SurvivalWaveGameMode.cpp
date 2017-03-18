// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SurvivalWave.h"
#include "SurvivalWaveGameMode.h"
#include "SurvivalWaveCharacter.h"

ASurvivalWaveGameMode::ASurvivalWaveGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

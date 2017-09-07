// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "InfoHUDClass.h"

UInfoHUDClass::UInfoHUDClass(const FObjectInitializer& init):Super(init) {

}

void UInfoHUDClass::SetInfo(FString Info) {
	InfoMsg = FText::FromString(Info);
	InfoUpdate();
}

void UInfoHUDClass::SetError(FString Info) {
	ErrorMsg = FText::FromString(Info);
	ErrorUpdate();
}

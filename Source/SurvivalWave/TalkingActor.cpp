// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "TalkingActor.h"


// Sets default values
ATalkingActor::ATalkingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BaseRoot"));
	RootComponent = RootArrow;

	CurrentTalkIndex = 0;
	bTextVisible = false;
}

// Called when the game starts or when spawned
void ATalkingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATalkingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATalkingActor::NextTalk() {
	if (CurrentTalkIndex >= 0) {
		if (Dialogue[CurrentTalkIndex].NextIndex >= 0 && !Dialogue[CurrentTalkIndex].bPassInput) {
			
			CurrentTalkIndex = Dialogue[CurrentTalkIndex].NextIndex;
			ChangeTalkBP();
		}
	}
}

void ATalkingActor::NextTalkInput() {
	if (CurrentTalkIndex >= 0) {
		if (Dialogue[CurrentTalkIndex].NextIndex >= 0 && Dialogue[CurrentTalkIndex].bPassInput) {

			CurrentTalkIndex = Dialogue[CurrentTalkIndex].NextIndex;
			ChangeTalkBP();
		}
	}
}

void ATalkingActor::DetectPlayer(bool bBeginOverlap) {
	bTextVisible = bBeginOverlap;
	if (bBeginOverlap) {
		ShowTalkBP(true);
	}
	else {
		ShowTalkBP(false);
		NextTalk();
	}
}

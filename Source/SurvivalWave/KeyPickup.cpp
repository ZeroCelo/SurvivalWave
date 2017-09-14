// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "KeyPickup.h"

#include "KeyItemInventory.h"
#include "SurvivalWaveCharacter.h"
#include "EngineGlobals.h"

AKeyPickup::AKeyPickup() {

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(100.0f);
	//CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AKeyPickup::KeyHit);

	KeyValue = 0.0f;
}

void AKeyPickup::BeginPlay() {
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AKeyPickup::KeyHit);

}

void AKeyPickup::WasCollected_Implementation() {
	Super::WasCollected_Implementation();

	//Destroy();
}

void AKeyPickup::KeyHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Keys Hit")));
	//UKeyItemInventory* Keys = OtherActor->FindComponentByClass<UKeyItemInventory>();

	/*if (Keys != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Keys Detect")));
		
	}*/

	ASurvivalWaveCharacter* play = Cast<ASurvivalWaveCharacter>(OtherActor);

	if (play != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Play Detect")));
		play->KeysStats->AddKey(KeyValue);
		play->UpdateHUDKeys();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWave.h"
#include "AIDroneFriendController.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAIDroneFriendController::AAIDroneFriendController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {

}

// Called when the game starts or when spawned
void AAIDroneFriendController::BeginPlay()
{
	Super::BeginPlay();


	GetWorld()->GetTimerManager().SetTimer(RefTimer, this, &AAIDroneFriendController::EnemyRefErrorCheck, 0.100f, false);
	GetWorld()->GetTimerManager().SetTimer(FollowTimer, this, &AAIDroneFriendController::UpdatePosition, 0.018f, true);
}

void AAIDroneFriendController::EnemyRefErrorCheck() {
	PawnRef = GetPawn();
	if (PawnRef != nullptr) {
		GetWorld()->GetTimerManager().ClearTimer(RefTimer);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Drone Friend yay...")));
	}

	TArray<AActor*> FoundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASurvivalWaveCharacter::StaticClass(), FoundPlayer);
	if (FoundPlayer.Num() > 0) {
		PlayerRef = Cast<ASurvivalWaveCharacter>(FoundPlayer[0]);
		if (PlayerRef != nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Friend Player yay...")));
		}
	}
}

void AAIDroneFriendController::UpdatePosition() {
	if (PlayerRef != nullptr && PawnRef != nullptr) {
		FVector Dist = PlayerRef->GetActorLocation() - PawnRef->GetActorLocation();
		if (Dist.Size() > 50.0f) {
			//PlayerRef->GetActorForwardVector();
			FRotator rot = PlayerRef->GetActorRotation();
			
			FVector trans(-50.0f, -50.0f, 135.0f);
			trans = rot.RotateVector(trans);
			FVector FollowPoint = PlayerRef->GetActorLocation() + trans;
			float Speed = 2.0f;
			FVector NewPos = FMath::Lerp(PawnRef->GetActorLocation(), FollowPoint, Speed * GetWorld()->GetDeltaSeconds());
			PawnRef->SetActorLocation(NewPos);
			FVector PlayerLook = PlayerRef->GetActorLocation() + FVector(0.0f, 0.0f, 75.0f);
			FRotator DroneRot = UKismetMathLibrary::FindLookAtRotation(PawnRef->GetActorLocation(), PlayerLook);
			PawnRef->SetActorRotation(DroneRot);
		}
	}
}

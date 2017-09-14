// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelDoor.h"

#include "KeySwitch.generated.h"

UCLASS()
class SURVIVALWAVE_API AKeySwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeySwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switch", meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* SwitchMesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	float KeyValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	float FindDoorTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	FString DoorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	bool bDisplayMsg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	bool bDoorOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgKeyNeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMessages")
	FText MsgKeyFound;

	ALevelDoor* Door;

	FTimerHandle FindTimer;

	void FindDoor();

	void CheckDoor();

	void PrintMessage(bool bHasKey);
	
	UFUNCTION(BlueprintCallable)
	void DoorCheck(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};

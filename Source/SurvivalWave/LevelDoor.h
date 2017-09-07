// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LevelDoor.generated.h"

UCLASS()
class SURVIVALWAVE_API ALevelDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Lock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* Front;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* Origin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	int32 PlayerNear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	bool bActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	bool bDoorClosed;

	//Make Door Never Lock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	bool bOpenBothSides;

	//If Door should wait for a level to load
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	bool bWaitLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	bool bLevelLoaded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	bool bLevelShouldUnload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	int32 NextRoomIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorMecanism")
	FString LevelStreamName;

	UFUNCTION(BlueprintCallable)
	FString GetLevelName();

	UFUNCTION(BlueprintCallable)
	int32 GetNextRoomIndex();

	//Checks wich direction the player exit, -1 returns if not a SurvivalWaveCharacter, [0 .. 1.45] == front exit
	UFUNCTION(BlueprintCallable)
	float CheckActorDirection(AActor* Other);
	
	UFUNCTION(BlueprintCallable)
	bool IsDoorClosed();

	UFUNCTION(BlueprintCallable)
	bool IsDoorActive();

	UFUNCTION(BlueprintCallable)
	bool IsDoorWaiting();

	UFUNCTION(BlueprintCallable)
	bool IsDoorLevelLoaded();

	UFUNCTION(BlueprintCallable)
	bool ShouldLevelUnload();

	UFUNCTION(BlueprintCallable)
	void SetLevelUnload(bool bnew);

	UFUNCTION(BlueprintCallable)
	void SetDoorActive(bool bnew);

	UFUNCTION(BlueprintCallable)
	void SetLevelLoaded(bool bnew);

	UFUNCTION(BlueprintCallable)
	void SetWaitLevel(bool bnew);

	UFUNCTION(BlueprintCallable)
	void CheckPlayer(AActor* Other);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetupActive();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CloseDoor();

	UFUNCTION(BlueprintCallable)
	void DetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void UnDetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

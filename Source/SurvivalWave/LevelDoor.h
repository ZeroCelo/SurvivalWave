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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mecanism")
	int32 PlayerNear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mecanism")
	bool bActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mecanism")
	bool bOpenBothSides;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetupActive();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CloseDoor();

	UFUNCTION(BlueprintCallable)
	bool IsDoorActive();

	UFUNCTION(BlueprintCallable)
	void SetDoorActive(bool bnew);

	UFUNCTION(BlueprintCallable)
	void CheckPlayer(AActor* Other);

	UFUNCTION(BlueprintCallable)
	void DetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void UnDetectPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

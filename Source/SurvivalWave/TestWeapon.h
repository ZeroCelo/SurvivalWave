// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"

#include "TestWeapon.generated.h"

UCLASS()
class SURVIVALWAVE_API ATestWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	class USkeletalMeshComponent* SkelMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	class UArrowComponent* FirePoint;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DamageStat.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALWAVE_API UDamageStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageRate;
		
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetDamage();
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetDamageRange();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetDamageRate();

	//Pass all variables to this component
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void CopyDamage(UDamageStat* dam);
	virtual void CopyDamage_Implementation(UDamageStat* dam);

};

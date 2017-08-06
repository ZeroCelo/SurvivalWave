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
		
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	float GetDamage();
	virtual float GetDamage_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	float GetDamageRange();
	virtual float GetDamageRange_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	float GetDamageRate();
	virtual float GetDamageRate_Implementation();

	//Pass all variables to this component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void CopyDamage(const UDamageStat* dam);
	virtual void CopyDamage_Implementation(const UDamageStat* dam);

};

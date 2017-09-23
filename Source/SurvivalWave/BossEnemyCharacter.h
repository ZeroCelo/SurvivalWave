// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"

#include "BossEnemyCharacter.generated.h"

//If you want this to appear in BP, make sure to use this instead
USTRUCT(BlueprintType)
struct FBossMinion
{
	GENERATED_BODY()

		//Always make USTRUCT variables into UPROPERTY()
		//any non-UPROPERTY() struct vars are not replicated
		//So to simplify your life for later debugging, always use UPROPERTY()
public:
	//If you want the property to appear in BP, make sure to use this instead
	//UPROPERTY(BlueprintReadOnly)
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> MinionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuantityMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuantityMax;
	
	//Constructor
	FBossMinion() {
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor

		QuantityMin = 1;
		QuantityMax = 2;
	}
};

/**
 * 
 */
UCLASS()
class SURVIVALWAVE_API ABossEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	ABossEnemyCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Messages")
	float IntimidateTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Messages")
	float IntimidateTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Messages")
	TArray<FText> IntimidateMsgIntro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Messages")
	TArray<FText> IntimidateMsg;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Messages")
	TArray<int32> IntimidatePreviousMsg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<FBossMinion> Minions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<class AProjectile> ProjectileAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float StopTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float StopTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float SpawnTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float SpawnTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float ProjectileTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float ProjectileTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float ProjectileTimeInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	int32 ProjectileMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* ProjectilePoint;

	FTimerHandle IntimidateTimer;
	FTimerHandle IntimidateIntroTimer;
	//FTimerHandle TestTimer;
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ProjectileAttack(FVector Offset, bool bUseOffset);
	
	void UpdateController();
	void SpawnMinions();
	void IntimidatePlayer();
	void IntimidatePlayerIntro();
	void CallIntimidate();
	void CallIntimidateIntro();
};

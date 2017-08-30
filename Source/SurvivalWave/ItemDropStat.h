// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ItemPickup.h"

#include "ItemDropStat.generated.h"

//If you want this to appear in BP, make sure to use this instead
USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	//Always make USTRUCT variables into UPROPERTY()
	//any non-UPROPERTY() struct vars are not replicated
	//So to simplify your life for later debugging, always use UPROPERTY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	//If you want the property to appear in BP, make sure to use this instead
	//UPROPERTY(BlueprintReadOnly)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuantityMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 QuantityMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ChancePoints;

	void SetQuantity(const int32 Min, const int32 Max) {
		QuantityMin = Min;
		QuantityMax = Max;
	}

	void SetChance(const int32 Pro) {
		ChancePoints = Pro;
	}

	void SetType(const EItemType NewType) {
		ItemType = NewType;
	}

	friend bool operator<(const FItemData& another,const FItemData& other) {
		return another.ChancePoints < other.ChancePoints;
	}
	
	friend bool operator==(const FItemData& first, const FItemData& second) {
		return first.ItemType == second.ItemType && first.ChancePoints == second.ChancePoints && first.QuantityMax == second.QuantityMax;
	}

	//Work as TMaps Key
	/*friend int32 GetTypeHash(const FItemData& other) {
	return GetTypeHash(other.id);
	}*/

	
	FORCEINLINE FItemData& operator=(const FItemData& other) {
		//if (this != &other) { //self assignment check
			ItemType = other.ItemType;
			QuantityMin = other.QuantityMin;
			QuantityMax = other.QuantityMax;
			ChancePoints = other.ChancePoints;
		//}
		return *this;
	}

	//Constructor
	FItemData() {
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		ItemType = EItemType::IT_AmmoR;
		QuantityMin = 1;
		QuantityMax = 30;
		ChancePoints = 50;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALWAVE_API UItemDropStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemDropStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Returns true if something was dropped
	UFUNCTION(BlueprintCallable)
	bool GetDrop();

	//Returns number of items to drop based on ItemsQuantity Array, Uses the ItemData.QuantityMax to determine the quantity
	UFUNCTION(BlueprintCallable)
	int32 GetDropQuantity();
	
	UFUNCTION(BlueprintCallable)
	FItem GetDropItem();

	UFUNCTION(BlueprintCallable)
	void DropPoints();

	//Maximum Probability(Pts) of something/nothing to be dropped
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxChance;

	//Probability(Pts) of nothing being dropped, Ex: Nothing=30 Max = 100, chance=30/100
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NothingChance;

	//What Items to Drop
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemData> ItemsDrop;

	//Chances of the Items Quantity to be dropped, Uses QuantityMax and ChancePoints only
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemData> ItemsQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> ScoreActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ScoreDropCount;
};

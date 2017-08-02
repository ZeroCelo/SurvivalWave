// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "ItemPickup.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemType : uint8
{
	IT_LifeS	UMETA(DisplayName = "LifeS"),
	IT_LifeM	UMETA(DisplayName = "LifeM"),
	IT_LifeL	UMETA(DisplayName = "LifeL"),
	IT_AmmoR	UMETA(DisplayName = "AmmoR"),
	IT_GunR		UMETA(DisplayName = "GunR"),
	IT_GunL		UMETA(DisplayName = "GunL")
};

//If you want this to appear in BP, make sure to use this instead
USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	//Always make USTRUCT variables into UPROPERTY()
	//any non-UPROPERTY() struct vars are not replicated
	//So to simplify your life for later debugging, always use UPROPERTY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType type;

	//If you want the property to appear in BP, make sure to use this instead
	//UPROPERTY(BlueprintReadOnly)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 limit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 id;

	static uint32 id_vault;

	void SetLimit(const int32 NewValue) {
		limit = NewValue;
	}

	void SetQuantity(const int32 NewValue) {
		if (NewValue <= limit)
			quantity = NewValue;
	}

	void SetType(const EItemType NewType) {
		type = NewType;
	}

	static FString GetItemEnumAsString(EItemType ty) {
		switch (ty) {
		case EItemType::IT_LifeS: return FString("IT_LifeS");
		case EItemType::IT_LifeM: return FString("IT_LifeM");
		case EItemType::IT_LifeL: return FString("IT_LifeL");
		case EItemType::IT_AmmoR: return FString("IT_AmmoR");
		case EItemType::IT_GunR: return FString("IT_GunR");
		case EItemType::IT_GunL: return FString("IT_GunL");
		default: return FString("IT_Invalid");
		}
	}

	int32 GetID() { return id; }

	FString GetNameID() {
		GenerateID();
		FString str = GetItemEnumAsString(type);
		str += '-';
		str += FString::FromInt(id);
		return str;
	}

	uint32 GenerateID() {
		if (id < 0) {
			id_vault++;
			id = id_vault;
		}
		return id;
	}

	//Work as TMaps Key
	friend bool operator==(const FItem& first, const FItem& second) {
		return first.id == second.id;
	}

	//Work as TMaps Key
	friend int32 GetTypeHash(const FItem& other) {
		return GetTypeHash(other.id);
	}

	FORCEINLINE FItem& operator=(const FItem& other) {
		if (this != &other) { //self assignment check
			this->limit = other.limit;
			this->quantity = other.quantity;
			this->type = other.type;
		}
		return *this;
	}

	//Constructor
	FItem(){
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		type = EItemType::IT_AmmoR;
		quantity = 30;
		limit = 90;
		id = -1;
	}
};


UCLASS()
class SURVIVALWAVE_API AItemPickup : public APickup
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AItemPickup();
	
	void WasCollected_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItem ItemStat;
	
};

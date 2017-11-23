// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"

#include "TalkingActor.generated.h"

//If you want this to appear in BP, make sure to use this instead
USTRUCT(BlueprintType)
struct FTalkText
{
	GENERATED_BODY()

	//Next Dialogue To change
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NextIndex;

	//Put here the lines of the text to show
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Lines;

	//Tell if we should pass with input or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPassInput;

	//Constructor
	FTalkText() {
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		bPassInput = true;
		NextIndex = -1;
	}
};

UCLASS()
class SURVIVALWAVE_API ATalkingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATalkingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTalkText> Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentTalkIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTextVisible;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* RootArrow;

	UFUNCTION(BlueprintCallable)
	void NextTalkInput();

	UFUNCTION(BlueprintCallable)
	void NextTalk();

	UFUNCTION(BlueprintCallable)
	void DetectPlayer(bool bBeginOverlap);

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void ShowTalkBP(bool bShowDialogue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeTalkBP();
};

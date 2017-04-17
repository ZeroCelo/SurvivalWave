// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
//#include "TestWeapon.h"

#include "SurvivalWaveCharacter.generated.h"

UCLASS(config=Game)
class ASurvivalWaveCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ASurvivalWaveCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Cam")
	void ChangeFOV(float new_fov);
	void UpdateFOV(float DeltaTime);
	void CheckFOV();
	void ChangeCamPos(FVector new_pos);
	void UpdateCamPos(float Deltatime);

	//Blueprint event to pass Run Animation variables
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAnimRun();

	//Blueprint event to pass Aim Animation variables
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void UpdateAnimAim();

	//Blueprint event to pass Fire Animation variables
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void UpdateAnimFire();

	//Blueprint event to pass Weapon Switch Animation variables
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void UpdateAnimSwitch();

	void EnableRun();
	void DisableRun();

	void EnableAim();
	void DisableAim();

	void EnableFire();
	void DisableFire();

	void PreviousGunPress();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void SwitchGunBP();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SwitchGun(int32 ind);

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void PickupWeapon(TSubclassOf<class ATestWeapon> WhatWeapon);

	//UFUNCTION(BlueprintCallable, Category = "Pickup")
	//void SetWeapon(int32 ind, ATestWeapon Weap) { Weapon[ind] = *Weap; }

	//Array of weapons that was pickup, null in case nothing was picked up, ToDo: 3rd is the infinite ammo weapon. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<ATestWeapon*> Weapon;
	//TSubclassOf<class ATestWeapon> Weapon1;
	//Selected Weapon Index
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 weapon_select;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool switching;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	bool firing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Moving")
	bool running;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Moving")
	bool aiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving")
	float speed_run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving")
	float speed_normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	float fov_normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	float fov_aim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	float fov_run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector cam_normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector cam_aim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector cam_run;

	//Maximum time in seconds for the Camera FOV/POS change animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	float fov_max_time;

	FVector cam_check;
	float fov_check;
	float fov_elapsed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Life")
	float life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	float life_max;
};


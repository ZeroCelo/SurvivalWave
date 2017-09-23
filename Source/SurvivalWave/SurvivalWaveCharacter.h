// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
//#include "ItemPickup.h"
#include "InventoryWidget.h"
#include "Weapon_Instant.h"
#include "LifeStat.h"
#include "Projectile.h"
#include "KeyItemInventory.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollectSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> ItemHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	class UUserWidget* ItemHUDWidget;
		
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD")
	void UpdateHUDItem();

	//If there is an item and CanInteract = true, show item in the HUD
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowHUDItem();

	//Function to get selected pickup
	UFUNCTION(BlueprintCallable)
	FItem GetPickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<int32, FItem> items;
	TMap<int32, AItemPickup*> items_actor;

	UFUNCTION(BlueprintCallable)
	void DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void PickupDetectionEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void PickupDetectionExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Function to update the camera possition, called in the tick for animation effect
	UFUNCTION(BlueprintCallable, Category = "Cam")
	void UpdateCam(float DeltaTime);
	
	//Event to check the player state(Aiming,running....) and update camera accordingly
	UFUNCTION(BlueprintCallable, Category = "Cam")
	void CheckCam();

	//Function to update the camera position and field of view
	UFUNCTION(BlueprintCallable, Category = "Cam")
	void ChangeCam(FVector new_pos,float new_fov);

	//Blueprint event to pass Run Animation variables
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Anim")
	void UpdateAnimRun();

	//Blueprint event to pass Aim Animation variables
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Anim")
	void UpdateAnimAim();

	//Blueprint event to pass Fire Animation variables
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Anim")
	void UpdateAnimFire();

	//Blueprint event to pass Weapon Switch Animation variables
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "Anim")
	void UpdateAnimSwitch();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Anim")
	void UpdateAnimReload();

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool CanRun();

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool CanAim();

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool CanFire();	

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool CanInventory();

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool CanInteract();

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool CanSwitch();

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool CanDropGun();

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool CanReloadGun();
	
	void EnableRun();
	void DisableRun();
	void CheckRun();

	void EnableAim();
	void DisableAim();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopFire();

	void PullTrigger();
	void ReleaseTrigger();

	void PausePress();
	void FirstGunPress();
	void SecondGunPress();
	void PistolGunPress();
	void NextGunPress();
	void PreviousGunPress();
	void DropGunPress();
	void ReloadGunPress();
	void InventoryPress();
	void InteractPress();

	float LastFireTime;
	FTimerHandle TriggerTimer;
	
	//Event to make Blueprint related things on this state context
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	void InventoryPressBP();

	//Event to make Blueprint related things on this state context
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	void InteractPressBP();

	//Event that handles the pause game state
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PausePressBP();

	//Event to make Blueprint related things on this state context
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pickup")
	void SwitchGunBP();

	//Event to make Blueprint related things on this state context
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pickup")
	void DropGunPressBP();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SwitchGun();
		
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetupWeaponBP();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetupWeaponIndexBP(int32 index);
	
	//Array of weapons that was pickup, null in case nothing was picked up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<AWeapon*> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TMap<EItemType,TSubclassOf<class AWeapon> > WeaponMapClass;

	//What Weapons to spawn at Begin Play, tip: use this to put the infinite gun class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<TSubclassOf<class AWeapon> > WeaponClassInit;

	//Selected Weapon Index
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 weapon_select;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 weapon_select_next;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	//int32 weapon_select_new;

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetSwitching(bool val) { bswitching = val; }

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetFiring(bool val) { bfiring = val; }

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetAiming(bool val) { baiming = val; }

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetRunning(bool val) { brunning = val; }

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetReloading(bool val) { bReloading = val; }
		
	UFUNCTION(BlueprintCallable, Category = "Getter")
	bool GetSwitching() { return bswitching; }

	UFUNCTION(BlueprintCallable, Category = "Getter")
	bool GetFiring() { return bfiring; }

	UFUNCTION(BlueprintCallable, Category = "Getter")
	bool GetRunning() { return brunning; }

	UFUNCTION(BlueprintCallable, Category = "Getter")
	bool GetAiming() { return baiming; }

	UFUNCTION(BlueprintCallable, Category = "Getter")
	bool GetInventory() { return binventory; }

	UFUNCTION(BlueprintCallable, Category = "Getter")
	bool GetReloading() { return bReloading; }
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bswitching;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	bool bfiring;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Moving")
	bool brunning;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Moving")
	bool baiming;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Moving")
	bool binventory;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	bool bReloading;

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
	float fov_inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector cam_normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector cam_aim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector cam_run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	FVector cam_inventory;

	//Maximum time in seconds for the Camera FOV/POS change animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cam")
	float fov_max_time;

	bool brun_press;
	float run_forward;

	FVector cam_check;
	float fov_check;
	float fov_elapsed;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> InventoryClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	class UUserWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> LifeHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	class UUserWidget* LifeHUDWidget;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD")
	void UpdateHUDLife();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD")
	void UpdateHUDWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> WeaponHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	class UUserWidget* WeaponHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	class ULifeStat* LifeStats;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD")
	void UpdateHUDKeys();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	class UKeyItemInventory* KeysStats;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Life")
	void GetKilled();

	UFUNCTION(BlueprintCallable, Category = "Life")
	void Death();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
	float DeathTime;

	FTimerHandle DeathTimer;
};


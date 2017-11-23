// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SurvivalWave.h"
//#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"
//#include "HeadMountedDisplayTypes.h"
#include "SurvivalWaveCharacter.h"
#include "TestWeapon.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"

//#include "EngineGlobals.h"
#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// ASurvivalWaveCharacter

ASurvivalWaveCharacter::ASurvivalWaveCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CollectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectSphere->SetupAttachment(RootComponent);
	CollectSphere->SetSphereRadius(200.0f);
	//CollectSphere->OnComponentBeginOverlap.AddDynamic(this, &ASurvivalWaveCharacter::PickupDetectionEnter);
	CollectSphere->OnComponentEndOverlap.AddDynamic(this, &ASurvivalWaveCharacter::PickupDetectionExit);

	LifeStats = CreateDefaultSubobject<ULifeStat>(TEXT("LifeComponent"));
	this->AddOwnedComponent(LifeStats);
	//LifeStats->SetupAttachment(RootComponent);

	KeysStats = CreateDefaultSubobject<UKeyItemInventory>(TEXT("KeysInventory"));
	this->AddOwnedComponent(KeysStats);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASurvivalWaveCharacter::DetectDamage);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	run_forward = 0.0f;
	brun_press = false;
	bRunning = false;
	bAiming = false;
	bFiring = false;
	bSwitching = false;
	bInventory = false;
	speed_run = 600.0f;
	speed_normal = 250.0f;
	fov_normal = 90.0f;
	//cam_normal = CameraBoom->SocketOffset;
	cam_normal = FVector(0.0f,50.0f,10.0f);
	fov_inventory = 100.0f;
	//cam_inventory = cam_normal;
	cam_inventory = FVector(0.0f, 150.0f, -40.0f);
	fov_aim = 70.0f;
	//cam_aim = cam_normal;
	cam_aim = FVector(50.0f, 50.0f, 0.0f);
	fov_run = 120.0f;
	//cam_run = cam_normal;
	cam_run = FVector(50.0f, 25.0f, -40.0f);
	cam_check = CameraBoom->SocketOffset;
	fov_check = fov_normal;
	fov_max_time = 0.750f;
	fov_elapsed = 100.0f;
	weapon_select = 0;
	DeathTime = 5.0f;
	DodgeDistance = 3000.0f;
	DodgeMaxTime = 2.0f;
	//weapon_select_new = 0;
	Weapon.Add(nullptr);
	Weapon.Add(nullptr);
	Weapon.Add(nullptr);

}

//////////////////////////////////////////////////////////////////////////
// Input

void ASurvivalWaveCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalWaveCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalWaveCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASurvivalWaveCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASurvivalWaveCharacter::LookUpAtRate);

	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ASurvivalWaveCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ASurvivalWaveCharacter::TouchStopped);

	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASurvivalWaveCharacter::OnResetVR);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASurvivalWaveCharacter::EnableRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASurvivalWaveCharacter::DisableRun);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ASurvivalWaveCharacter::EnableAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ASurvivalWaveCharacter::DisableAim);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASurvivalWaveCharacter::PullTrigger);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASurvivalWaveCharacter::ReleaseTrigger);
	PlayerInputComponent->BindAction("PreviousGun", IE_Pressed, this, &ASurvivalWaveCharacter::PreviousGunPress);
	PlayerInputComponent->BindAction("NextGun", IE_Pressed, this, &ASurvivalWaveCharacter::NextGunPress);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ASurvivalWaveCharacter::InventoryPress);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASurvivalWaveCharacter::InteractPress);
	PlayerInputComponent->BindAction("DropGun", IE_Pressed, this, &ASurvivalWaveCharacter::DropGunPress);
	PlayerInputComponent->BindAction("ReloadGun", IE_Pressed, this, &ASurvivalWaveCharacter::ReloadGunPress);
	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &ASurvivalWaveCharacter::PausePress);
	PlayerInputComponent->BindAction("SelectWeapon1", IE_Pressed, this, &ASurvivalWaveCharacter::FirstGunPress);
	PlayerInputComponent->BindAction("SelectWeapon2", IE_Pressed, this, &ASurvivalWaveCharacter::SecondGunPress);
	PlayerInputComponent->BindAction("SelectWeaponPistol", IE_Pressed, this, &ASurvivalWaveCharacter::PistolGunPress);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ASurvivalWaveCharacter::DodgePress);
}

void ASurvivalWaveCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASurvivalWaveCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASurvivalWaveCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASurvivalWaveCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalWaveCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalWaveCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr))
	//if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Fire DirForward %f,%f,%f (%f)"), Direction.X,Direction.Y,Direction.Z,Value));

		//if(Value >= 0.0f)
			//CheckRun();
		if(bRunning && Value < 0.0f)
			AddMovementInput(Direction, 0.0f);
		else
			AddMovementInput(Direction, Value); 


		/*if (Value <= 0.0f) {
			run_forward = 0.0f;
		}
		
		if (!bRunning) {
			AddMovementInput(Direction, Value);
			if (brun_press && Value > 0.0f) {
				run_forward = Value;
				//EnableRun();
			}
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DirForward %f"), Value));
			if (Value >= 0.9f) {
				AddMovementInput(Direction, Value);
			}
		}*/
	}
}

void ASurvivalWaveCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !bRunning)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Fire DirRight %f,%f,%f (%f)"), Direction.X, Direction.Y, Direction.Z,Value));
		if (!bRunning)
			AddMovementInput(Direction, Value);
		else
			TurnAtRate(Value);
	}
}

void ASurvivalWaveCharacter::BeginPlay() {
	Super::BeginPlay();
	EnableInput(Cast<APlayerController>(GetController()));

	//Setup the Inventory Widget
	if (InventoryClass != nullptr) {
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryClass);
		if (InventoryWidget != nullptr) {
			InventoryWidget->AddToViewport();
			InventoryWidget->RemoveFromParent();
		}
		//if (gameHUD != nullptr) gameHUD->AddToViewport();
	}
	//Setup Item Interaction widget
	if (ItemHUDClass != nullptr) {
		ItemHUDWidget = CreateWidget<UUserWidget>(GetWorld(), ItemHUDClass);
	}
	//Setup Life Component widget
	if (LifeHUDClass != nullptr) {
		LifeHUDWidget = CreateWidget<UUserWidget>(GetWorld(), LifeHUDClass);
		if (LifeHUDWidget != nullptr) {
			LifeHUDWidget->AddToViewport();
			UpdateHUDLife();
			UpdateHUDShield();
		}
	}
	//Setup Shield Check
	if (LifeStats != nullptr) {
		//GetWorld()->GetTimerManager().SetTimer(ShieldTimer, this, &ASurvivalWaveCharacter::UpdateHUDShield, LifeStats->ShieldRechargeRate, true);
		GetWorld()->GetTimerManager().SetTimer(ShieldTimer, this, &ASurvivalWaveCharacter::UpdateHUDShield, 0.03f, true);
	}
	//Setup Weapon Information widget
	if (WeaponHUDClass != nullptr) {
		WeaponHUDWidget = CreateWidget<UUserWidget>(GetWorld(), WeaponHUDClass);
		if (WeaponHUDWidget != nullptr) {
			WeaponHUDWidget->AddToViewport();
			UpdateHUDWeapon();
		}
	}
	for (int32 i = 0; i < WeaponClassInit.Num() && (i - 1) < Weapon.Num(); i++) {
	//if(Weapon_Class.Num() > 0){
		if (WeaponClassInit[i] != nullptr) {
		//if (Weapon_Class[0] != nullptr) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			

			//Get a random location to spawn at
			FVector SpawnLocation = RootComponent->GetComponentLocation();

			//get a random rotation for the spawned item
			FRotator SpawnRotation = RootComponent->GetComponentRotation();
			Weapon[i] = GetWorld()->SpawnActor<AWeapon>(WeaponClassInit[i]);
			int32 weapon_i = i - 1;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Setup Weapon UI try")));
			if (InventoryWidget != nullptr) {
				UInventoryWidget* itemUI = Cast<UInventoryWidget>(InventoryWidget);
				if (itemUI != nullptr) {
					if (weapon_i < itemUI->Weapons.Num() && weapon_i >= 0) {
						itemUI->Weapons[weapon_i].id = 1;
						itemUI->Weapons[weapon_i].quantity = 1;
						itemUI->Weapons[weapon_i].limit = 1;
						itemUI->Weapons[weapon_i].type = Weapon[i]->WeaponType;
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Setup Weapon UI")));
					}
				}
			}
		}
	}
	SetupWeaponBP();
}

void ASurvivalWaveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCam(DeltaTime);
	DodgeElapsedTime += DeltaTime;
	//UpdateDeathCam(DeltaTime);
	//UpdateFOV(DeltaTime);
	//UpdateCamPos(DeltaTime);
}

bool ASurvivalWaveCharacter::CanRun() {
	return (!bSwitching && !bReloading && !bInventory && brun_press);
}

bool ASurvivalWaveCharacter::CanAim() {
	return !bInventory;
}

bool ASurvivalWaveCharacter::CanFire() {
	return (!bSwitching && !bInventory && !bReloading);
}

bool ASurvivalWaveCharacter::CanInventory() {
	return (!bFiring && !bAiming && !bRunning && !bReloading);
}

bool ASurvivalWaveCharacter::CanInteract() {
	return (!bInventory);
}

bool ASurvivalWaveCharacter::CanSwitch() {
	return (!bSwitching && !bReloading);
}

bool ASurvivalWaveCharacter::CanDropGun() {
	return (!bRunning && !bAiming && !bReloading);
}

bool ASurvivalWaveCharacter::CanReloadGun() {
	return (!bRunning && !bInventory && !bSwitching && !bReloading);
}

bool ASurvivalWaveCharacter::CanDodge() {
	return (!bRunning && !bInventory && !bSwitching && !bReloading && !bAiming);
}

void ASurvivalWaveCharacter::CheckRun() {
	if (CanRun()) {
		ReleaseTrigger();
		bRunning = true;
		bAiming = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Run")));
		GetCharacterMovement()->MaxWalkSpeed = speed_run;
		//FollowCamera->FieldOfView = fov_normal;
		//float cur_speed = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
		//if(cur_speed > 10.0f)ChangeFOV(fov_run);
		//FollowCamera->FieldOfView = fov_run;
		UpdateAnimRun();
		UpdateAnimAim();
		//CheckFOV();
		CheckCam();
	}
}

void ASurvivalWaveCharacter::EnableRun() {
	brun_press = true;
	//UpdateAnimRun();
	//UpdateAnimAim();
	//CheckCam();
	if (CanRun()) {
		ReleaseTrigger();
		bRunning = true;
		bAiming = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Run")));
		GetCharacterMovement()->MaxWalkSpeed = speed_run;
		//FollowCamera->FieldOfView = fov_normal;
		//float cur_speed = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
		//if(cur_speed > 10.0f)ChangeFOV(fov_run);
		//FollowCamera->FieldOfView = fov_run;
		UpdateAnimRun();
		UpdateAnimAim();
		//CheckFOV();
		CheckCam();
	}
}

void ASurvivalWaveCharacter::DisableRun() {
	brun_press = false;
	bRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = speed_normal;
	//FollowCamera->FieldOfView = fov_normal;
	//if(!aiming)ChangeFOV(fov_normal);
	UpdateAnimRun();
	//CheckFOV();
	CheckCam();
}

void ASurvivalWaveCharacter::EnableAim() {
	if (CanAim()) {
		bAiming = true;
		bRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = speed_normal;
		//FollowCamera->FieldOfView = fov_aim;
		//ChangeFOV(fov_aim);

		UpdateAnimAim();
		UpdateAnimRun();
		//CheckFOV();
		CheckCam();
	}
}

void ASurvivalWaveCharacter::DisableAim() {
	bAiming = false;
	//FollowCamera->FieldOfView = fov_normal;
	//if(!running)ChangeFOV(fov_normal);
	UpdateAnimAim();
	// CheckFOV();
	CheckCam();
}

void ASurvivalWaveCharacter::PullTrigger() {
	if (CanFire()) {
		DisableRun();
		bFiring = true;
		if (Weapon[weapon_select] != nullptr) {
			if(Weapon[weapon_select]->TargetCamera == nullptr)
				Weapon[weapon_select]->TargetCamera = FollowCamera;

			Weapon[weapon_select]->StartFire();
			GetWorld()->GetTimerManager().SetTimer(TriggerTimer, this, &ASurvivalWaveCharacter::UpdateHUDWeapon, 0.15f, true);
		}
		UpdateAnimFire();
		UpdateHUDWeapon();
	}
}

void ASurvivalWaveCharacter::ReleaseTrigger() {
	bFiring = false;
	if (Weapon[weapon_select] != nullptr)
		Weapon[weapon_select]->StopFire();
	GetWorld()->GetTimerManager().ClearTimer(TriggerTimer);
	UpdateAnimFire();
	UpdateHUDWeapon();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UnFire")));
}

void ASurvivalWaveCharacter::StartFire() {
	bFiring = true;
	/*if (Weapon[weapon_select] != nullptr) {
		
		float game_time = GetWorld()->GetTimeSeconds();
		float dif_time = game_time - LastFireTime;
		if (dif_time >= Weapon[weapon_select]->DamageStats->GetDamageRate()) {
			LastFireTime = game_time;
			Weapon[weapon_select]->StartFire();
		}
		GetWorld()->GetTimerManager().SetTimer(TriggerTimer, this, &ASurvivalWaveCharacter::StartFire, LastFireTime + Weapon[weapon_select]->DamageStats->GetDamageRate() - game_time, false);
		//GetWorld()->GetTimerManager().SetTimer(Weapon[weapon_select]->MuzzleTimer, this, &ASurvivalWaveCharacter::StopFire, Weapon[weapon_select]->muzzle_time, false);
	}*/
	UpdateAnimFire();
	UpdateHUDWeapon();
}

void ASurvivalWaveCharacter::StopFire() {
	bFiring = false;

	UpdateAnimFire();
	UpdateHUDWeapon();
}

void ASurvivalWaveCharacter::SwitchGun() {
	int32 temp = weapon_select;
	weapon_select = weapon_select_next;
	weapon_select_next = temp;
	SwitchGunBP();
}

void ASurvivalWaveCharacter::FirstGunPress() {
	if (CanSwitch() && weapon_select != 1) {
		if (Weapon[1] != nullptr) {
			weapon_select_next = 1;
			bSwitching = true;
			ReleaseTrigger();
			//DisableAim();
			DisableRun();
			UpdateAnimSwitch();
		}
	}
}

void ASurvivalWaveCharacter::SecondGunPress() {
	if (CanSwitch() && weapon_select != 2) {
		if (Weapon[2] != nullptr) {
			weapon_select_next = 2;
			bSwitching = true;
			ReleaseTrigger();
			//DisableAim();
			DisableRun();
			UpdateAnimSwitch();
		}
	}
}

void ASurvivalWaveCharacter::PistolGunPress() {
	if (CanSwitch() && weapon_select != 0) {
		if (Weapon[0] != nullptr) {
			weapon_select_next = 0;
			bSwitching = true;
			ReleaseTrigger();
			//DisableAim();
			DisableRun();
			UpdateAnimSwitch();
		}
	}
}

void ASurvivalWaveCharacter::NextGunPress() {
	if (CanSwitch()) {
		weapon_select_next = weapon_select + 1;
		do {
			if (weapon_select_next >= Weapon.Num())weapon_select_next = 0;
			if (Weapon[weapon_select_next] != nullptr) {
				if (weapon_select_next == weapon_select) break;
				bSwitching = true;
				ReleaseTrigger();
				//DisableAim();
				DisableRun();

				UpdateAnimSwitch();
				break;
			}
			else weapon_select_next++;
		} while (1);
	}
}

void ASurvivalWaveCharacter::PreviousGunPress() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PreviousGun %d Num %d"), weapon_select, weapon_select_next));
	if (CanSwitch()) {
		weapon_select_next = weapon_select - 1;
		do {
			if (weapon_select_next < 0)weapon_select_next = Weapon.Num() - 1;
			if (Weapon[weapon_select_next] != nullptr) {
				if (weapon_select_next == weapon_select) break;
				bSwitching = true;
				ReleaseTrigger();
				//DisableAim();
				DisableRun();

				UpdateAnimSwitch();
				break;
			}
			else weapon_select_next--;
		} while (1);
	}
}

void ASurvivalWaveCharacter::DropGunPress() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DropGun")));
	if (CanDropGun()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DropGun Try")));
		if (weapon_select > 0) {
			UInventoryWidget* itemUI = Cast<UInventoryWidget>(InventoryWidget);
			if (itemUI != nullptr) {
				itemUI->Weapons[weapon_select - 1].quantity = Weapon[weapon_select]->GetWeaponAmmo();
				itemUI->Weapons[weapon_select - 1].limit = Weapon[weapon_select]->GetWeaponAmmo();
			}
			Weapon[weapon_select]->Destroy();
			Weapon[weapon_select] = nullptr;
			DropGunPressBP();
			NextGunPress();
		}
	}
}

void ASurvivalWaveCharacter::InventoryPress() {
	if (CanInventory()) {
		bInventory = !bInventory;
		CheckCam();
		UpdateHUDItem();
		ShowHUDItem();
		InventoryPressBP();
	}
}

void ASurvivalWaveCharacter::ReloadGunPress() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Reload")));
	if (CanReloadGun()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Reloading...")));
		UInventoryWidget* itemUI = Cast<UInventoryWidget>(InventoryWidget);
		if (Weapon[weapon_select] != nullptr && itemUI != nullptr) {
			int32 ammo_sum = itemUI->GetItemSum(Weapon[weapon_select]->GetWeaponAmmoType());
			if (Weapon[weapon_select]->ShouldReload() && Weapon[weapon_select]->IsReloadInfinite()) {
				Weapon[weapon_select]->ReloadAmmo(Weapon[weapon_select]->GetWeaponAmmoMax());
				bReloading = true;
				ReleaseTrigger();
				DisableRun();
				UpdateAnimReload();
			}
			else if (Weapon[weapon_select]->ShouldReload() && !Weapon[weapon_select]->IsReloadInfinite() && ammo_sum > 0) {
				
				int32 ammo_need = Weapon[weapon_select]->GetWeaponAmmoMax() - Weapon[weapon_select]->GetWeaponAmmo();
				if (ammo_sum > ammo_need) {
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Reloading...Consume Remain")));
					Weapon[weapon_select]->ReloadAmmo(ammo_need);
					itemUI->ConsumeItem(Weapon[weapon_select]->GetWeaponAmmoType(),ammo_need);
					bReloading = true;
					ReleaseTrigger();
					DisableRun();
					UpdateAnimReload();
				}
				else {
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Reloading...Consume All")));
					Weapon[weapon_select]->ReloadAmmo(ammo_sum);
					itemUI->ConsumeItem(Weapon[weapon_select]->GetWeaponAmmoType(), ammo_sum);
					bReloading = true;
					ReleaseTrigger();
					DisableRun();
					UpdateAnimReload();
				}
			}
		}
	}
}

void ASurvivalWaveCharacter::InteractPress() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Interact")));
	if (CanInteract()) {

		if (items.Num()) {	//Have item to pickup
			FItem it = items.CreateConstIterator()->Value;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Item id%d quant%d limit%d type:%s"), it.id,it.quantity, it.limit, *FItem::GetItemEnumAsString(it.type)));
			UInventoryWidget* itemUI = Cast<UInventoryWidget>(InventoryWidget);
			if (itemUI != nullptr) {
				FString str = FItem::GetItemEnumAsString(it.type);
				if (str.Contains("Gun")) {
					for (int32 i = 0;i < Weapon.Num();i++) {
						if (Weapon[i] == nullptr && WeaponMapClass.Contains(it.type)) {
							Weapon[i] = GetWorld()->SpawnActor<AWeapon>(WeaponMapClass[it.type]);
							Weapon[i]->Ammo = it.quantity;
							SetupWeaponIndexBP(i);
							break;
						}
					}
				}
				int32 consume = itemUI->AddItem(it);
				AItemPickup* temp = items_actor.Find(it.id)[0];
				if (consume > 0) {
					if (temp != nullptr) {
						temp->ItemStat.quantity = consume;
						items[it.id].quantity = consume;
					}
				}
				else {
					if (temp != nullptr) {
						items.Remove(it.id);
						items_actor.Remove(it.id);
						temp->WasCollected();
					}
				}
			}
		}
		InteractPressBP();
	}
}

void ASurvivalWaveCharacter::PausePress() {
	PausePressBP();
}

void ASurvivalWaveCharacter::DodgePress() {
	if (CanDodge()) {
		if (DodgeElapsedTime > DodgeMaxTime) {
			DodgeElapsedTime = 0.0f;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Dodging!")));

			FVector Direction = GetVelocity();
			Direction.Normalize();

			FVector start = GetActorLocation();
			FVector end = start + Direction*DodgeDistance;

			LaunchCharacter(end, false, false);
			//GetCharacterMovement()->GroundFriction = 0.0f;
			//GetCharacterMovement()->AddImpulse(end, true);
			//GetWorld()->GetTimerManager().SetTimer(DodgeTimer, this, &ASurvivalWaveCharacter::DodgeReset, DodgeResetTime, false);
			DodgePressBP();
		}
	}
}

void ASurvivalWaveCharacter::ChangeCam(FVector new_pos, float new_fov) {
	fov_check = new_fov;
	cam_check = new_pos;
	fov_elapsed = 0.0f;
	//GetWorld()->GetTimerManager().SetTimer(fov_timer, this, &ASurvivalWaveCharacter::UpdateFOV, 1.0f / fov_cnt_max, true);
}

void ASurvivalWaveCharacter::CheckCam() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vel %f,%f,%f"), GetVelocity().X,GetVelocity().Y, GetVelocity().Z));

	if (GetAiming() && !GetRunning()) {
		ChangeCam(cam_aim, fov_aim);
	}
	else if (!GetAiming() && GetRunning()) {
		ChangeCam(cam_run, fov_run);
	}
	else if (GetAiming() && GetRunning()) {
		if (GetCharacterMovement()->MaxWalkSpeed == speed_run) {
			ChangeCam(cam_run, fov_run);
		}
		else {
			ChangeCam(cam_aim, fov_aim);
		}
	}
	else if (GetInventory()) {
		ChangeCam(cam_inventory, fov_inventory);
	}
	else {
		ChangeCam(cam_normal, fov_normal);
	}
}

void ASurvivalWaveCharacter::UpdateCam(float DeltaTime) {
	if (fov_elapsed < fov_max_time) {
		float part = fov_check - FollowCamera->FieldOfView;
		FVector part2 = cam_check - CameraBoom->SocketOffset;
		part /= fov_max_time;
		part2 /= fov_max_time;
		float remain = fov_elapsed / fov_max_time;
		FollowCamera->FieldOfView += remain*part;
		CameraBoom->SocketOffset += remain*part2;
		fov_elapsed += DeltaTime;		
	}
}

void ASurvivalWaveCharacter::ShowHUDItem() {
	if (items.Num() && CanInteract()) {
		if (ItemHUDWidget != nullptr) {
			if (!ItemHUDWidget->IsInViewport())ItemHUDWidget->AddToViewport();
		}
	}
	else {
		if (ItemHUDWidget != nullptr) {
			if (ItemHUDWidget->IsInViewport())ItemHUDWidget->RemoveFromViewport();
		}
	}
}

void ASurvivalWaveCharacter::PickupDetectionEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vel %f,%f,%f"), GetVelocity().X, GetVelocity().Y, GetVelocity().Z));
	
	FString str("None");
	AItemPickup* item = Cast<AItemPickup>(OtherActor);
	if (item != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Detect id%d quant%d limit%d type:%s"), item->ItemStat.id, item->ItemStat.quantity, item->ItemStat.limit, *FItem::GetItemEnumAsString(item->ItemStat.type)));
		//item->SetupSelection(true);
		str = item->ItemStat.GetNameID();
		int32 item_id = item->ItemStat.GetID();
		if (!items.Contains(item_id)) {
			items.Add(item_id, item->ItemStat);
			items_actor.Add(item_id,item);
		}
	}
	UpdateHUDItem();
	ShowHUDItem();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Pickup Enter %s"),*str));
	
}

void ASurvivalWaveCharacter::PickupDetectionExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vel %f,%f,%f"), GetVelocity().X, GetVelocity().Y, GetVelocity().Z));
	FString str("None");
	AItemPickup* item = Cast<AItemPickup>(OtherActor);
	if (item != nullptr) {
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Detect id%d quant%d limit%d type:%s"), item->ItemStat.id, item->ItemStat.quantity, item->ItemStat.limit, *FItem::GetItemEnumAsString(item->ItemStat.type)));
		item->SetupSelection(false);
		str = item->ItemStat.GetNameID();
		int32 item_id = item->ItemStat.GetID();
		if (items.Contains(item_id)) {
			items.Remove(item_id);
			items_actor.Remove(item_id);
		}
	}
	//if( CanInteract()) 
	UpdateHUDItem();
	ShowHUDItem();
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Pickup Exit %s"), *str));

	//uint32 act_address = (uint32)(OtherActor);
}

void ASurvivalWaveCharacter::DetectDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LifeStats->DetectDamage(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UpdateHUDLife();
	UpdateHUDShield();
	if (LifeStats->IsDead()) {
		//DisableInput(Cast<APlayerController>(GetController()));
		if (InventoryWidget != nullptr)InventoryWidget->RemoveFromParent();
		if (ItemHUDWidget != nullptr)ItemHUDWidget->RemoveFromParent();
		if (WeaponHUDWidget != nullptr)WeaponHUDWidget->RemoveFromParent();
		if (LifeHUDWidget != nullptr)LifeHUDWidget->RemoveFromParent();
		//GetMesh()->SetSimulatePhysics(true);
		GetKilled();
		GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ASurvivalWaveCharacter::Death, DeathTime,false);
	}
}

void ASurvivalWaveCharacter::Death() {
	Destroy();
}

FItem ASurvivalWaveCharacter::GetPickup() {
	if (items.Num()) {
		//AItemPickup* temp = items_actor.Find(items.CreateConstIterator()->Value.id)[0];
		//temp->SetupSelection(true);
		for (auto& temp : items_actor) {
			if (temp.Key == items.CreateConstIterator()->Value.id)
				temp.Value->SetupSelection(true);
			else
				temp.Value->SetupSelection(false);
		}
		
		return items.CreateConstIterator()->Value;
	}
	FItem ret; ret.quantity = -1;
	return ret;
}

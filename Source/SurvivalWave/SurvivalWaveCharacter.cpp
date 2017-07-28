// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SurvivalWave.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "SurvivalWaveCharacter.h"
#include "TestWeapon.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	run_forward = 0.0f;
	brun_press = false;
	brunning = false;
	baiming = false;
	bfiring = false;
	bswitching = false;
	binventory = false;
	speed_run = 600.0f;
	speed_normal = 250.0f;
	fov_normal = 90.0f;
	cam_normal = CameraBoom->SocketOffset;
	fov_inventory = 90.0f;
	cam_inventory = cam_normal;
	fov_aim = 60.0f;
	cam_aim = cam_normal;
	fov_run = 120.0f;
	cam_run = cam_normal;
	cam_check = CameraBoom->SocketOffset;
	fov_check = fov_normal;
	fov_max_time = 1.0f;
	fov_elapsed = 100.0f;
	life_max = 100.0f;
	life = life_max;
	weapon_select = 1;
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
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASurvivalWaveCharacter::EnableFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASurvivalWaveCharacter::DisableFire);
	PlayerInputComponent->BindAction("PreviousGun", IE_Pressed, this, &ASurvivalWaveCharacter::PreviousGunPress);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ASurvivalWaveCharacter::InventoryPress);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASurvivalWaveCharacter::InteractPress);
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
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Fire DirForward %f,%f,%f (%f)"), Direction.X,Direction.Y,Direction.Z,Value));

		if (Value <= 0.0f) {
			run_forward = 0.0f;
		}

		if (!brunning) {
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
		}
	}
}

void ASurvivalWaveCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !brunning)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Fire DirRight %f,%f,%f (%f)"), Direction.X, Direction.Y, Direction.Z,Value));
		if (!brunning)
			AddMovementInput(Direction, Value);
		else
			TurnAtRate(Value);
	}
}

void ASurvivalWaveCharacter::BeginPlay() {
	Super::BeginPlay();
	//Setup the Inventory Widget
	if (InventoryClass != nullptr) {
		inventory_widget = CreateWidget<UUserWidget>(GetWorld(), InventoryClass);
		
		//if (gameHUD != nullptr) gameHUD->AddToViewport();
	}
	if (ItemHUDClass != nullptr) {
		ItemHUDWidget = CreateWidget<UUserWidget>(GetWorld(), ItemHUDClass);
	}
}

void ASurvivalWaveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCam(DeltaTime);
	//UpdateFOV(DeltaTime);
	//UpdateCamPos(DeltaTime);
}

bool ASurvivalWaveCharacter::CanRun() {
	return (!bswitching && !binventory && run_forward > 0.90f);
}

bool ASurvivalWaveCharacter::CanAim() {
	return !binventory;
}

bool ASurvivalWaveCharacter::CanFire() {
	return (!bswitching && !binventory);
}

bool ASurvivalWaveCharacter::CanInventory() {
	return (!bfiring && !baiming && !brunning);
}

bool ASurvivalWaveCharacter::CanInteract() {
	return (!binventory);
}

bool ASurvivalWaveCharacter::CanSwitch() {
	return (!bswitching);
}

void ASurvivalWaveCharacter::EnableRun() {
	brun_press = true;
	if (CanRun()) {
		DisableFire();
		brunning = true;
		baiming = false;
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
	brunning = false;
	GetCharacterMovement()->MaxWalkSpeed = speed_normal;
	//FollowCamera->FieldOfView = fov_normal;
	//if(!aiming)ChangeFOV(fov_normal);
	UpdateAnimRun();
	//CheckFOV();
	CheckCam();
}

void ASurvivalWaveCharacter::EnableAim() {
	if (CanAim()) {
		baiming = true;
		brunning = false;
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
	baiming = false;
	//FollowCamera->FieldOfView = fov_normal;
	//if(!running)ChangeFOV(fov_normal);
	UpdateAnimAim();
	// CheckFOV();
	CheckCam();
}

void ASurvivalWaveCharacter::EnableFire() {
	if (CanFire()) {
		DisableRun();
		bfiring = true;
		if (Weapon[weapon_select] != nullptr)
			Weapon[weapon_select]->StartFire();
		UpdateAnimFire();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Fire")));
	}
}

void ASurvivalWaveCharacter::DisableFire() {
	bfiring = false;
	if (Weapon[weapon_select] != nullptr)
		Weapon[weapon_select]->StopFire();
	UpdateAnimFire();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UnFire")));
}

void ASurvivalWaveCharacter::SwitchGun(int32 ind) {
	weapon_select = weapon_select % 2;
	weapon_select++;
	SwitchGunBP();
}

void ASurvivalWaveCharacter::InventoryPress() {
	if (CanInventory()) {
		binventory = !binventory;
		CheckCam();
		UpdateHUDItem();
		ShowHUDItem();
		InventoryPressBP();
	}
}

void ASurvivalWaveCharacter::InteractPress() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Interact")));
	if (CanInteract()) {

		if (items.Num()) {	//Have item to pickup
			FItem it = items.CreateConstIterator()->Value;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Item id%d quant%d limit%d type:%s"), it.id,it.quantity, it.limit, *FItem::GetItemEnumAsString(it.type)));
			UInventoryWidget* itemUI = Cast<UInventoryWidget>(inventory_widget);
			if (itemUI != nullptr) {
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

void ASurvivalWaveCharacter::PreviousGunPress() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SwitchGun")));
	if (CanSwitch()) {
		bswitching = true;
		DisableFire();
		//DisableAim();
		DisableRun();
		UpdateAnimSwitch();
	}
}

void ASurvivalWaveCharacter::PickupWeapon(TSubclassOf<class ATestWeapon> WhatWeapon) {
	
}

void ASurvivalWaveCharacter::ChangeCam(FVector new_pos, float new_fov) {
	fov_check = new_fov;
	cam_check = new_pos;
	fov_elapsed = 0.0f;
	//GetWorld()->GetTimerManager().SetTimer(fov_timer, this, &ASurvivalWaveCharacter::UpdateFOV, 1.0f / fov_cnt_max, true);
}

void ASurvivalWaveCharacter::CheckCam() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vel %f,%f,%f"), GetVelocity().X,GetVelocity().Y, GetVelocity().Z));
	//float cur_speed = FVector::DotProduct(vel_temp, GetActorRotation().Vector());

	if (baiming && !brunning) {
		ChangeCam(cam_aim, fov_aim);
	}
	else if (!baiming && brunning) {
		ChangeCam(cam_run, fov_run);
	}
	else if (baiming && brunning) {
		if (GetCharacterMovement()->MaxWalkSpeed == speed_run) {
			ChangeCam(cam_run, fov_run);
		}
		else {
			ChangeCam(cam_aim, fov_aim);
		}
	}
	else if (binventory) {
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
		str = item->ItemStat.GetNameID();
		int32 item_id = item->ItemStat.GetID();
		if (!items.Contains(item_id)) {
			items.Add(item_id, item->ItemStat);
			items_actor.Add(item_id,item);
		}
	}
	//if (items.Num() > 0 && CanInteract()) {
	UpdateHUDItem();
	ShowHUDItem();
	//}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Pickup Enter %s"),*str));
	
}

void ASurvivalWaveCharacter::PickupDetectionExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vel %f,%f,%f"), GetVelocity().X, GetVelocity().Y, GetVelocity().Z));
	FString str("None");
	AItemPickup* item = Cast<AItemPickup>(OtherActor);
	if (item != nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Detect id%d quant%d limit%d type:%s"), item->ItemStat.id, item->ItemStat.quantity, item->ItemStat.limit, *FItem::GetItemEnumAsString(item->ItemStat.type)));
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

FItem ASurvivalWaveCharacter::GetPickup() {
	if (items.Num()) {
		return items.CreateConstIterator()->Value;
	}
	FItem ret; ret.quantity = -1;
	return ret;
}

// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SurvivalWave.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "SurvivalWaveCharacter.h"
#include "TestWeapon.h"

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	running = false;
	aiming = false;
	firing = false;
	speed_run = 600.0f;
	speed_normal = 250.0f;
	fov_normal = 90.0f;
	fov_aim = 60.0f;
	fov_run = 120.0f;
	fov_check = 90.0f;
	fov_max_time = 1.0f;
	fov_elapsed = fov_max_time;
	life_max = 100.0f;
	life = life_max;
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
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Fire DirForward %f,%f,%f"), Direction.X,Direction.Y,Direction.Z));
		if (!running) {
			AddMovementInput(Direction, Value);
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DirForward %f"), Value));
			if(Value >= 0.9f)
				AddMovementInput(Direction, Value);
		}
	}
}

void ASurvivalWaveCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !running)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Fire DirRight %f,%f,%f"), Direction.X, Direction.Y, Direction.Z));
		if(!running)
			AddMovementInput(Direction, Value);
	}
}

void ASurvivalWaveCharacter::BeginPlay() {
	Super::BeginPlay();
	//DisableRun();
}

void ASurvivalWaveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateFOV(DeltaTime);
}

void ASurvivalWaveCharacter::EnableRun() {
	DisableFire();
	running = true;
	aiming = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Run")));
	GetCharacterMovement()->MaxWalkSpeed = speed_run;
	//FollowCamera->FieldOfView = fov_normal;
	ChangeFOV(fov_run);
	//FollowCamera->FieldOfView = fov_run;
	UpdateAnimRun();
	UpdateAnimAim();
}

void ASurvivalWaveCharacter::DisableRun() {
	running = false;
	GetCharacterMovement()->MaxWalkSpeed = speed_normal;
	//FollowCamera->FieldOfView = fov_normal;
	if(!aiming)ChangeFOV(fov_normal);
	UpdateAnimRun();
}

void ASurvivalWaveCharacter::EnableAim() {
	aiming = true;
	running = false;
	GetCharacterMovement()->MaxWalkSpeed = speed_normal;
	//FollowCamera->FieldOfView = fov_aim;
	ChangeFOV(fov_aim);
	
	UpdateAnimAim();
	UpdateAnimRun();
}

void ASurvivalWaveCharacter::DisableAim() {
	aiming = false;
	//FollowCamera->FieldOfView = fov_normal;
	if(!running)ChangeFOV(fov_normal);
	UpdateAnimAim();
}

void ASurvivalWaveCharacter::EnableFire() {
	DisableRun();
	firing = true;
	if (Weapon1 != nullptr)
		Weapon1->StartFire();
	UpdateAnimFire();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Fire")));
}

void ASurvivalWaveCharacter::DisableFire() {
	firing = false;
	if (Weapon1 != nullptr)
		Weapon1->StopFire();
	UpdateAnimFire();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UnFire")));
}

void ASurvivalWaveCharacter::PickupWeapon(TSubclassOf<class ATestWeapon> WhatWeapon) {

}

void ASurvivalWaveCharacter::ChangeFOV(float new_fov) {
	fov_check = new_fov;
	fov_elapsed = 0.0f;
	//GetWorld()->GetTimerManager().SetTimer(fov_timer, this, &ASurvivalWaveCharacter::UpdateFOV, 1.0f / fov_cnt_max, true);
}

void ASurvivalWaveCharacter::UpdateFOV(float DeltaTime) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("FOV %d"),fov_cnt));

	if (fov_elapsed < fov_max_time) {
		float part = fov_check - FollowCamera->FieldOfView;
		part /= fov_max_time;
		float remain = fov_elapsed / fov_max_time;
		FollowCamera->FieldOfView += remain*part;
		fov_elapsed += DeltaTime;
	}
}

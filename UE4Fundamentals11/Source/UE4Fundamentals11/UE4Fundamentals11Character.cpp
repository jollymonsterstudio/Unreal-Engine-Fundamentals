// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UE4Fundamentals11Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Props/InteractiveProp.h"

#include "UI/InGameHUD.h"

#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// AUE4Fundamentals11Character

AUE4Fundamentals11Character::AUE4Fundamentals11Character()
{
	PrimaryActorTick.bCanEverTick = true;

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

	// load player attack montage data table
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerAttackMontageDataObject(TEXT("DataTable'/Game/TUTORIAL_RESOURCES/DataTables/PlayerAttackMontageDataTable.PlayerAttackMontageDataTable'"));
	if (PlayerAttackMontageDataObject.Succeeded())
	{
		PlayerAttackDataTable = PlayerAttackMontageDataObject.Object;
	}

	// load the sound cue object
	static ConstructorHelpers::FObjectFinder<USoundCue> PunchSoundCueObject(TEXT("SoundCue'/Game/TUTORIAL_RESOURCES/Audio/PunchSoundCue.PunchSoundCue'"));
	if (PunchSoundCueObject.Succeeded())
	{
		PunchSoundCue = PunchSoundCueObject.Object;

		PunchAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PunchAudioComponent"));
		PunchAudioComponent->SetupAttachment(RootComponent);
	}

	// load the punch throw sound cue object
	static ConstructorHelpers::FObjectFinder<USoundCue> PunchThrowSoundCueObject(TEXT("SoundCue'/Game/TUTORIAL_RESOURCES/Audio/PunchThrowSoundCue.PunchThrowSoundCue'"));
	if (PunchThrowSoundCueObject.Succeeded())
	{
		PunchThrowSoundCue = PunchThrowSoundCueObject.Object;

		PunchThrowAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PunchThrowAudioComponent"));
		PunchThrowAudioComponent->SetupAttachment(RootComponent);
	}
	
	LeftMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftMeleeCollisionBox"));
	LeftMeleeCollisionBox->SetupAttachment(RootComponent);
	LeftMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(false);

	RightMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightMeleeCollisionBox"));
	RightMeleeCollisionBox->SetupAttachment(RootComponent);
	RightMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(false);

	// set animation blending on by default
	bIsAnimationBlended = true;

	LineTraceType = ELineTraceType::CAMERA_SINGLE;
	LineTraceDistance = 0.f;

	CurrentAttackStrength = EAttackStrength::MEDIUM;

	CurrentCombo = 0;

	MaxCountdownToIdle = 30;

	MaxCrouchSpeed = 200.f;
	MaxWalkSpeed = 250.f;
	MaxRunSpeed = 600.f;
	MaxArmedSpeed = 200.f;

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxCrouchSpeed;
}

void AUE4Fundamentals11Character::BeginPlay()
{
	Super::BeginPlay();

	LeftMeleeCollisionBox->OnComponentHit.AddDynamic(this, &AUE4Fundamentals11Character::OnAttackHit);
	RightMeleeCollisionBox->OnComponentHit.AddDynamic(this, &AUE4Fundamentals11Character::OnAttackHit);

	// make sure our audio variables are initialized
	if (PunchSoundCue && PunchAudioComponent) {
		// attach the sound cue to our audio component 
		// NOTE: do not do this in the constructor as it will play the sound when the player spawns
		PunchAudioComponent->SetSound(PunchSoundCue);
	}

	// make sure our audio variables are initialized
	if (PunchThrowSoundCue && PunchThrowAudioComponent) {
		// attach the sound cue to our audio component 
		// NOTE: do not do this in the constructor as it will play the sound when the player spawns
		PunchThrowAudioComponent->SetSound(PunchThrowSoundCue);
	}
}

void AUE4Fundamentals11Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE4Fundamentals11Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up game play key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUE4Fundamentals11Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUE4Fundamentals11Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUE4Fundamentals11Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUE4Fundamentals11Character::LookUpAtRate);

	PlayerInputComponent->BindAction("ArmPlayer", IE_Pressed, this, &AUE4Fundamentals11Character::ArmPlayerImmediately);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AUE4Fundamentals11Character::CrouchStart);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AUE4Fundamentals11Character::CrouchEnd);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AUE4Fundamentals11Character::RunStart);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AUE4Fundamentals11Character::RunEnd);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUE4Fundamentals11Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUE4Fundamentals11Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUE4Fundamentals11Character::OnResetVR);

	// attack functionality
	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AUE4Fundamentals11Character::PunchAttack);
	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &AUE4Fundamentals11Character::KickAttack);

	PlayerInputComponent->BindAction("LightAttackModifier", IE_Pressed, this, &AUE4Fundamentals11Character::LightAttackStart);
	PlayerInputComponent->BindAction("LightAttackModifier", IE_Released, this, &AUE4Fundamentals11Character::LightAttackEnd);

	PlayerInputComponent->BindAction("StrongAttackModifier", IE_Pressed, this, &AUE4Fundamentals11Character::StrongAttackStart);
	PlayerInputComponent->BindAction("StrongAttackModifier", IE_Released, this, &AUE4Fundamentals11Character::StrongAttackEnd);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUE4Fundamentals11Character::Interact);
	
}

void AUE4Fundamentals11Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUE4Fundamentals11Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AUE4Fundamentals11Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AUE4Fundamentals11Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUE4Fundamentals11Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUE4Fundamentals11Character::MoveForward(float Value)
{
	MoveForwardValue = Value;

	if (Value != 0.0f && bIsKeyboardEnabled)
	{
		if(bIsArmed && !bIsCrouched)
		{
			if (!GetCharacterMovement()->bUseControllerDesiredRotation)
			{
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
				GetCharacterMovement()->bOrientRotationToMovement = false;
			}

			AddMovementInput(GetActorForwardVector(), Value);
		}
		else
		{
			if (GetCharacterMovement()->bUseControllerDesiredRotation)
			{
				GetCharacterMovement()->bUseControllerDesiredRotation = false;
				GetCharacterMovement()->bOrientRotationToMovement = true;
			}

			// find out which way is forward
			const FRotator Rotation = GetFollowCamera()->GetForwardVector().Rotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}		
	}
}

void AUE4Fundamentals11Character::MoveRight(float Value)
{
	MoveRightValue = Value;

	if (Value != 0.0f && bIsKeyboardEnabled )
	{
		if(bIsArmed && !bIsCrouched)
		{
			if (!GetCharacterMovement()->bUseControllerDesiredRotation)
			{
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
				GetCharacterMovement()->bOrientRotationToMovement = false;
			}
			
			// move -1 or +1 
			AddMovementInput(GetActorRightVector(), Value);
		}
		else
		{
			if (GetCharacterMovement()->bUseControllerDesiredRotation)
			{
				GetCharacterMovement()->bUseControllerDesiredRotation = false;
				GetCharacterMovement()->bOrientRotationToMovement = true;
			}

			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

bool AUE4Fundamentals11Character::IsRunning()
{
	return bIsRunning;
}

float AUE4Fundamentals11Character::GetMoveRight()
{
	return MoveRightValue;
}

float AUE4Fundamentals11Character::GetMoveForward()
{
	return MoveForwardValue;
}

bool AUE4Fundamentals11Character::IsAnimationBlended()
{
	return bIsAnimationBlended;
}

bool AUE4Fundamentals11Character::IsArmed()
{
	return bIsArmed;
}

void AUE4Fundamentals11Character::SetIsKeyboardEnabled(bool Enabled)
{
	bIsKeyboardEnabled = Enabled;
}

EAttackType AUE4Fundamentals11Character::GetCurrentAttack()
{
	return CurrentAttack;
}

void AUE4Fundamentals11Character::PunchAttack()
{
	AttackInput(EAttackType::MELEE_FIST);
}

void AUE4Fundamentals11Character::KickAttack()
{
	AttackInput(EAttackType::MELEE_KICK);
}

void AUE4Fundamentals11Character::AttackInput(EAttackType AttackType)
{
	//Log(ELogLevel::INFO, __FUNCTION__);

	CurrentAttackStrength = EAttackStrength::MEDIUM;
	if (bIsAttackStrong)
	{
		CurrentAttackStrength = EAttackStrength::STRONG;
	}
	else if (bIsAttackLight && !bIsAttackStrong) 
	{
		CurrentAttackStrength = EAttackStrength::LIGHT;
	}

	if (PlayerAttackDataTable)
	{
		static const FString ContextString(TEXT("Player Attack Montage Context"));

		FName RowKey;

		// attach collision components to sockets based on transformations definitions
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

		CurrentAttack = AttackType;

		switch (AttackType)
		{
		case EAttackType::MELEE_FIST:
			RowKey = FName(TEXT("Punch"));

			LeftMeleeCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "fist_l_collision");
			RightMeleeCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "fist_r_collision");

			bIsKeyboardEnabled = true;

			bIsAnimationBlended = true;
			break;
		case EAttackType::MELEE_KICK:
			RowKey = FName(TEXT("Kick"));

			LeftMeleeCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "foot_l_collision");
			RightMeleeCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "foot_r_collision");

			bIsKeyboardEnabled = false;

			bIsAnimationBlended = false;
			break;
		default:

			bIsAnimationBlended = true;
			break;
		}

		AttackMontage = PlayerAttackDataTable->FindRow<FPlayerAttackMontage>(RowKey, ContextString, true);
		if (AttackMontage)
		{
			// pick the correct montage section based on our attack type
			int MontageSectionIndex;
			switch (CurrentAttackStrength)
			{
			case EAttackStrength::LIGHT:
				MontageSectionIndex = 1;
				break;
			case EAttackStrength::MEDIUM:
				MontageSectionIndex = 2;
				break;
			case EAttackStrength::STRONG:
				MontageSectionIndex = 3;
				break;
			default:
				MontageSectionIndex = 2;
				break;
			}

			// create a montage section
			FString MontageSection = "start_" + FString::FromInt(MontageSectionIndex);

			PlayAnimMontage(AttackMontage->Montage, 1.f, FName(*MontageSection));

			if (!bIsArmed)
			{
				bIsArmed = true;
			}
		}
	}
}

void AUE4Fundamentals11Character::AttackStart()
{
	//Log(ELogLevel::INFO, __FUNCTION__);

	LeftMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Enabled);
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(true);

	RightMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Enabled);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(true);

	ArmPlayer(true);
}

void AUE4Fundamentals11Character::AttackEnd()
{
	//Log(ELogLevel::INFO, __FUNCTION__);

	LeftMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(false);

	RightMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(false);

	//const bool bIsActive = GetWorld()->GetTimerManager().IsTimerActive(ArmedToIdleTimerHandle);
	//if (bIsActive)
	//{
	//	// reset the timer
	//	GetWorld()->GetTimerManager().ClearTimer(ArmedToIdleTimerHandle);
	//}
	//
	//CountdownToIdle = MaxCountdownToIdle;
	//// start timer from scratch
	//GetWorld()->GetTimerManager().SetTimer(ArmedToIdleTimerHandle, this, &AUE4Fundamentals11Character::TriggerCountdownToIdle, 1.f, true);
}

void AUE4Fundamentals11Character::LightAttackStart()
{
	bIsAttackLight = true;
}

void AUE4Fundamentals11Character::LightAttackEnd()
{
	bIsAttackLight = false;
}

void AUE4Fundamentals11Character::StrongAttackStart()
{
	bIsAttackStrong = true;
}

void AUE4Fundamentals11Character::StrongAttackEnd()
{
	bIsAttackStrong = false;
}

void AUE4Fundamentals11Character::OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Log(ELogLevel::WARNING, __FUNCTION__);
	//Log(ELogLevel::WARNING, Hit.GetActor()->GetName());

	// check to make sure the audio component is initialized and we are not already playing a sound
	if (PunchAudioComponent && !PunchAudioComponent->IsPlaying())
	{
		// activate the sound if it has not been already
		if (!PunchAudioComponent->IsActive()) 
		{
			PunchAudioComponent->Activate(true);
		}
		// default pitch value 1.0f
		// modify the pitch to create variance by grabbing a random float between 1.0 and 1.3
		PunchAudioComponent->SetPitchMultiplier(FMath::RandRange(1.0f, 1.3f));
		// play the sound
		PunchAudioComponent->Play(0.f);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// kind of working, animation still moves through target but blends back out on hit and looks almost natural
		//AnimInstance->Montage_Stop(AnimationVariable, AttackMontage->Montage);

		// note pause as it will provide some fun functionality in later tutorials
		//AnimInstance->Montage_Pause(AttackMontage->Montage);

		
		AnimInstance->Montage_Play(AttackMontage->Montage, AnimationVariable, EMontagePlayReturnType::Duration, AnimInstance->Montage_GetPosition(AttackMontage->Montage), true);
	}

	// update hud
	AInGameHUD* InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (InGameHUD)
	{
		CurrentCombo += 1;

		Log(ELogLevel::WARNING, __FUNCTION__);
		InGameHUD->UpdateComboCount(CurrentCombo);

		// make sure it's not active
		if (!GetWorld()->GetTimerManager().IsTimerActive(ComboResetTimerHandle)) {
			GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &AUE4Fundamentals11Character::ResetCombo, 5.1f, true);
		}
		else {
			GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &AUE4Fundamentals11Character::ResetCombo, 5.1f, true);
		}
	}

	AttackEnd();
}


void AUE4Fundamentals11Character::ResetCombo()
{
	CurrentCombo = 0;
	// update hud
	AInGameHUD* InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (InGameHUD)
	{
		InGameHUD->ResetCombo();
	}
}

void AUE4Fundamentals11Character::TriggerCountdownToIdle()
{
	Log(ELogLevel::WARNING, FString::FromInt(CountdownToIdle));

	// count down to zero
	if (--CountdownToIdle <= 0) {
		bIsArmed = false;
		CountdownToIdle = MaxCountdownToIdle;
		GetWorld()->GetTimerManager().ClearTimer(ArmedToIdleTimerHandle);
	}
}

void AUE4Fundamentals11Character::ArmPlayerImmediately()
{
	// flip flop the values
	ArmPlayer(!bIsArmed);
}

void AUE4Fundamentals11Character::ArmPlayer(bool Value)
{
	// flip / flop
	bIsArmed = Value;
	
	if (!bIsArmed)
	{
		CountdownToIdle = MaxCountdownToIdle;
		GetWorld()->GetTimerManager().ClearTimer(ArmedToIdleTimerHandle);

		if (bIsRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxArmedSpeed;
	}
}

void AUE4Fundamentals11Character::RunStart()
{
	if(!bIsCrouched && !bIsArmed)
	{
		bIsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
	}
}

void AUE4Fundamentals11Character::RunEnd()
{
	if (!bIsCrouched && !bIsArmed)
	{
		bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
}

void AUE4Fundamentals11Character::CrouchStart()
{
	Log(ELogLevel::WARNING, __FUNCTION__);

	if (bIsArmed)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		GetCharacterMovement()->MaxWalkSpeedCrouched = MaxArmedSpeed;
	} 
	else
	{
		GetCharacterMovement()->MaxWalkSpeedCrouched = MaxCrouchSpeed;
	}

	Crouch();
}

void AUE4Fundamentals11Character::CrouchEnd()
{
	Log(ELogLevel::WARNING, __FUNCTION__);

	if (bIsArmed)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	UnCrouch();
}

void AUE4Fundamentals11Character::Interact()
{
	FVector Start;
	FVector End;

	FVector PlayerEyesLoc;
	FRotator PlayerEyesRot;

	GetActorEyesViewPoint(PlayerEyesLoc, PlayerEyesRot);

	Start = PlayerEyesLoc;
	End = PlayerEyesLoc + (PlayerEyesRot.Vector() * LineTraceDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, this);

	FHitResult InteractHit = FHitResult(ForceInit);

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(InteractHit, Start, End, ECC_GameTraceChannel3, TraceParams);

	if(bIsHit && InteractHit.GetActor() != this)
	{
		// Log(ELogLevel::WARNING, InteractHit.Actor->GetName());
		// start to end, green, will lines always stay on, depth priority, thickness of line
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);

		// implements interface
		if(InteractHit.GetActor()->GetClass()->ImplementsInterface(UInteractiveActor::StaticClass()))
		{
			IInteractiveActor::Execute_Interact(InteractHit.GetActor());
		}
		else if(InteractHit.GetActor()->IsA(ACharacter::StaticClass()))
		{
			// check to see if we are a possessed entity
			if(bIsCurrentlyPossessed)
			{
				bIsCurrentlyPossessed = false;

				if(DefaultMaterialBody)
				{
					GetMesh()->SetMaterial(0, DefaultMaterialBody);	
				}
				if(DefaultMaterialChest)
				{
					GetMesh()->SetMaterial(1, DefaultMaterialChest);	
				}
			}

			AUE4Fundamentals11Character* PossessableCharacter = Cast<AUE4Fundamentals11Character>(InteractHit.GetActor());
			if(PossessableCharacter)
			{
				if(!PossessableCharacter->bIsCurrentlyPossessed)
				{
					// handle possession
					if(!SavedController)
					{
						// save the controller
						SavedController = GetController();	
					}

					// unpossess first ( helps with multiplayer )
					SavedController->UnPossess();

					// disable current player state management
					bIsKeyboardEnabled = false;					
					bIsRunning = false;
					bIsArmed = false;
					// disable movement mode
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
					
					// possess our new actor
					SavedController->Possess(Cast<APawn>(InteractHit.GetActor()));
					// enable movement back on the possessed actor
					PossessableCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					
					// set the values for our possession materials
					if(PossessableCharacter->PossessMaterialBody)
					{
						PossessableCharacter->GetMesh()->SetMaterial(0, PossessableCharacter->PossessMaterialBody);	
					}

					if(PossessableCharacter->PossessMaterialChest)
					{
						PossessableCharacter->GetMesh()->SetMaterial(1, PossessableCharacter->PossessMaterialChest);	
					}	

					// ensure the new player is correctly marked as possesed and can be interacted with
					PossessableCharacter->bIsKeyboardEnabled = true;
					PossessableCharacter->bIsCurrentlyPossessed = true;
				}
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
	}
}

void AUE4Fundamentals11Character::FireLineTrace()
{
	Log(ELogLevel::WARNING, __FUNCTION__);

	FVector Start;
	FVector End;

	// convert the degrees to radians
	const float Spread = FMath::DegreesToRadians(LineTraceSpread * 0.5f);

	if (LineTraceType == ELineTraceType::CAMERA_SINGLE || LineTraceType == ELineTraceType::CAMERA_SCATTER)
	{
		// get the camera view
		FVector CameraLoc = FollowCamera->GetComponentLocation();
		FRotator CameraRot = FollowCamera->GetComponentRotation();

		Start = CameraLoc;

		// add in distance to our look locations
		if (LineTraceType == ELineTraceType::CAMERA_SCATTER)
		{
			// handle spread
			End = CameraLoc + FMath::VRandCone(CameraRot.Vector(), Spread, Spread) * LineTraceDistance;
		}
		else {
			End = CameraLoc + (CameraRot.Vector() * LineTraceDistance);
		}
	}
	// handle player view of the same thing
	else if (LineTraceType == ELineTraceType::PLAYER_SINGLE || LineTraceType == ELineTraceType::PLAYER_SCATTER) 
	{
		FVector PlayerEyesLoc;
		FRotator PlayerEyesRot;

		// get the point of view of the actor
		GetActorEyesViewPoint(PlayerEyesLoc, PlayerEyesRot);

		Start = PlayerEyesLoc;

		// handle spread
		if (LineTraceType == ELineTraceType::PLAYER_SCATTER)
		{
			End = PlayerEyesLoc + FMath::VRandCone(PlayerEyesRot.Vector(), Spread, Spread) * LineTraceDistance;
		}
		else {
			End = PlayerEyesLoc + (PlayerEyesRot.Vector() * LineTraceDistance);
		}
	}

	
	// additional trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	//Re-initialize hit info
	FHitResult HitDetails = FHitResult(ForceInit);

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(
		HitDetails,				//	FHitResult object that will be populated with hit info
		Start,					//	starting position
		End,					//	end position
		ECC_GameTraceChannel3,	//	collision channel
		TraceParams				//	additional trace settings
	);

	if (bIsHit)
	{
		Log(ELogLevel::WARNING, "We hit something");
		// start to end, green, will lines always stay on, depth priority, thickness of line
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);

		Log(ELogLevel::WARNING, HitDetails.Actor->GetName());
		Log(ELogLevel::DEBUG, FString::SanitizeFloat(HitDetails.Distance));
		DrawDebugBox(GetWorld(), HitDetails.ImpactPoint, FVector(2.f, 2.f, 2.f), FColor::Blue, false, 5.f, ECC_WorldStatic, 1.f);

	}
	else
	{
		Log(ELogLevel::WARNING, "Nothing was hit");
		// start to end, purple, will lines always stay on, depth priority, thickness of line
		DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
	}
}



void AUE4Fundamentals11Character::Log(ELogLevel LogLevel, FString Message)
{
	Log(LogLevel, Message, ELogOutput::ALL);
}

void AUE4Fundamentals11Character::Log(ELogLevel LogLevel, FString Message, ELogOutput LogOutput)
{
	// only print when screen is selected and the GEngine object is available
	if ((LogOutput == ELogOutput::ALL || LogOutput == ELogOutput::SCREEN) && GEngine) 
	{
		// default color
		FColor LogColor = FColor::Cyan;
		// flip the color based on the type
		switch (LogLevel)
		{
		case ELogLevel::TRACE:
			LogColor = FColor::Green;
			break;
		case ELogLevel::DEBUG:
			LogColor = FColor::Cyan;
			break;
		case ELogLevel::INFO:
			LogColor = FColor::White;
			break;
		case ELogLevel::WARNING:
			LogColor = FColor::Yellow;
			break;
		case ELogLevel::ERROR:
			LogColor = FColor::Red;
			break;
		default:
			break;
		}
		// print the message and leave it on screen ( 4.5f controls the duration )
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, LogColor, Message);
	}

	if (LogOutput == ELogOutput::ALL || LogOutput == ELogOutput::OUTPUT_LOG)
	{
		// flip the message type based on error level
		switch (LogLevel)
		{
		case ELogLevel::TRACE:
			UE_LOG(LogTemp, VeryVerbose, TEXT("%s"), *Message);
			break;
		case ELogLevel::DEBUG:
			UE_LOG(LogTemp, Verbose, TEXT("%s"), *Message);
			break;
		case ELogLevel::INFO:
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
			break;
		case ELogLevel::WARNING:
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
			break;
		case ELogLevel::ERROR:
			UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
			break;
		}
	}	
}

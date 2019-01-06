// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UE4Fundamentals05Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// AUE4Fundamentals05Character

AUE4Fundamentals05Character::AUE4Fundamentals05Character()
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
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
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

	LeftMeleeCollisionBox->SetHiddenInGame(false);

	RightMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightMeleeCollisionBox"));
	RightMeleeCollisionBox->SetupAttachment(RootComponent);
	RightMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(false);

	RightMeleeCollisionBox->SetHiddenInGame(false);

	// set animation blending on by default
	IsAnimationBlended = true;

	LineTraceType = ELineTraceType::CAMERA_SINGLE;
	LineTraceDistance = 0.f;
}

void AUE4Fundamentals05Character::BeginPlay()
{
	Super::BeginPlay();

	LeftMeleeCollisionBox->OnComponentHit.AddDynamic(this, &AUE4Fundamentals05Character::OnAttackHit);
	RightMeleeCollisionBox->OnComponentHit.AddDynamic(this, &AUE4Fundamentals05Character::OnAttackHit);

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

void AUE4Fundamentals05Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// enable to fire all the time
	//FireLineTrace();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE4Fundamentals05Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up game play key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUE4Fundamentals05Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUE4Fundamentals05Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUE4Fundamentals05Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUE4Fundamentals05Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUE4Fundamentals05Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUE4Fundamentals05Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUE4Fundamentals05Character::OnResetVR);

	// attack functionality
	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AUE4Fundamentals05Character::PunchAttack);
	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &AUE4Fundamentals05Character::KickAttack);

	PlayerInputComponent->BindAction("FireTrace", IE_Pressed, this, &AUE4Fundamentals05Character::FireLineTrace);
	
}

void AUE4Fundamentals05Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUE4Fundamentals05Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AUE4Fundamentals05Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AUE4Fundamentals05Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUE4Fundamentals05Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUE4Fundamentals05Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && IsKeyboardEnabled)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUE4Fundamentals05Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && IsKeyboardEnabled )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

bool AUE4Fundamentals05Character::GetIsAnimationBlended()
{
	return IsAnimationBlended;
}

void AUE4Fundamentals05Character::SetIsKeyboardEnabled(bool Enabled)
{
	IsKeyboardEnabled = Enabled;
}

EAttackType AUE4Fundamentals05Character::GetCurrentAttack()
{
	return CurrentAttack;
}

void AUE4Fundamentals05Character::PunchAttack()
{
	AttackInput(EAttackType::MELEE_FIST);
}

void AUE4Fundamentals05Character::KickAttack()
{
	AttackInput(EAttackType::MELEE_KICK);
}

void AUE4Fundamentals05Character::AttackInput(EAttackType AttackType)
{
	//Log(ELogLevel::INFO, __FUNCTION__);

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

			IsKeyboardEnabled = true;

			IsAnimationBlended = true;
			break;
		case EAttackType::MELEE_KICK:
			RowKey = FName(TEXT("Kick"));

			LeftMeleeCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "foot_l_collision");
			RightMeleeCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "foot_r_collision");

			IsKeyboardEnabled = false;

			IsAnimationBlended = false;
			break;
		default:

			IsAnimationBlended = true;
			break;
		}

		AttackMontage = PlayerAttackDataTable->FindRow<FPlayerAttackMontage>(RowKey, ContextString, true);
		if (AttackMontage)
		{
			// generate  number between 1 and whatever is defined in the data table for this montage :
			int MontageSectionIndex = rand() % AttackMontage->AnimSectionCount + 1;

			// create a montage section
			FString MontageSection = "start_" + FString::FromInt(MontageSectionIndex);

			PlayAnimMontage(AttackMontage->Montage, 1.f, FName(*MontageSection));
		}
	}
}

void AUE4Fundamentals05Character::AttackStart()
{
	//Log(ELogLevel::INFO, __FUNCTION__);

	LeftMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Enabled);
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(true);

	RightMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Enabled);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(true);
}

void AUE4Fundamentals05Character::AttackEnd()
{
	//Log(ELogLevel::INFO, __FUNCTION__);

	LeftMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(false);

	RightMeleeCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
}

void AUE4Fundamentals05Character::OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
}


void AUE4Fundamentals05Character::FireLineTrace()
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
	FHitResult HitOut = FHitResult(ForceInit);



	bool bIsHit = GetWorld()->LineTraceSingleByChannel(
		HitOut,					//	FHitResult object that will be populated with hit info
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
	}
	else
	{
		Log(ELogLevel::WARNING, "Nothing was hit");
		// start to end, purple, will lines always stay on, depth priority, thickness of line
		DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
	}
}



void AUE4Fundamentals05Character::Log(ELogLevel LogLevel, FString Message)
{
	Log(LogLevel, Message, ELogOutput::ALL);
}

void AUE4Fundamentals05Character::Log(ELogLevel LogLevel, FString Message, ELogOutput LogOutput)
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

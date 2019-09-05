// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

#include "Engine/DataTable.h"

#include "UE4Fundamentals11Character.generated.h"

USTRUCT(BlueprintType)
struct FPlayerAttackMontage : public FTableRowBase
{
	GENERATED_BODY()

	/** Melee Fist Attack Montage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* Montage;

	/** amount of start sections within our montage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 AnimSectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Description;
};

USTRUCT(BlueprintType)
struct FMeleeCollisionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Disabled;

	// default constructor
	FMeleeCollisionProfile()
	{
		Enabled = FName(TEXT("Weapon"));
		Disabled = FName(TEXT("NoCollision"));
	}
};

UENUM(BlueprintType)
enum class ELogLevel: uint8 {
	TRACE			UMETA(DisplayName = "Trace"),
	DEBUG			UMETA(DisplayName = "Debug"),
	INFO			UMETA(DisplayName = "Info"),
	WARNING			UMETA(DisplayName = "Warning"),
	ERROR			UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class ELogOutput: uint8 {
	ALL				UMETA(DisplayName = "All levels"),
	OUTPUT_LOG		UMETA(DisplayName = "Output log"),
	SCREEN			UMETA(DisplayName = "Screen")
};

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	MELEE_FIST			UMETA(DisplayName = "Melee - Fist"),
	MELEE_KICK			UMETA(DisplayName = "Melee - Kick")
};

UENUM(BlueprintType)
enum class EAttackStrength: uint8 {
	LIGHT		UMETA(DisplayName = "Light"),
	MEDIUM		UMETA(DisplayName = "Medium ( Default )"),
	STRONG		UMETA(DisplayName = "Strong")
};

UENUM(BlueprintType)
enum class ELineTraceType : uint8 {
	CAMERA_SINGLE	UMETA(DisplayName = "Camera - Single Line"),
	PLAYER_SINGLE	UMETA(DisplayName = "Player - Single Line"),
	CAMERA_SCATTER	UMETA(DisplayName = "Camera - Scattering"),
	PLAYER_SCATTER	UMETA(DisplayName = "Player - Scattering")
};

UCLASS(config=Game)
class AUE4Fundamentals11Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** melee attack data table **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UDataTable* PlayerAttackDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* PunchSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* PunchThrowSoundCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* LeftMeleeCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* RightMeleeCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		float AnimationVariable;

	UPROPERTY(EditDefaultsOnly, Category = Possession)
	class UMaterialInterface* DefaultMaterialBody;
	UPROPERTY(EditDefaultsOnly, Category = Possession)
	class UMaterialInterface* DefaultMaterialChest;
	UPROPERTY(EditDefaultsOnly, Category = Possession)
	class UMaterialInterface* PossessMaterialBody;
	UPROPERTY(EditDefaultsOnly, Category = Possession)
	class UMaterialInterface* PossessMaterialChest;
	
	
public:
	AUE4Fundamentals11Character();

	// Called when the game starts or when the player is spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PunchAttack();
	void KickAttack();

	/**
	* Triggers attack animations based on user input
	*/
	void AttackInput(EAttackType AttackType);

	/**
	* Initiates player attack
	*/
	void AttackStart();

	/**
	* Stops player attack
	*/
	void AttackEnd();

	void LightAttackStart();
	void LightAttackEnd();
	void StrongAttackStart();
	void StrongAttackEnd();

	/**
	* Triggered when the collision hit event fires between our weapon and enemy entities
	*/
	UFUNCTION()
	void OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** boolean that tells us if we need to branch our animation blue print paths **/
	UFUNCTION(BlueprintCallable, Category=Animation)
	bool IsAnimationBlended();
	
	UFUNCTION(BlueprintCallable, Category = Animation)
	bool IsArmed();

	UFUNCTION(BlueprintCallable, Category = Animation)
	bool IsRunning();

	UFUNCTION(BlueprintCallable, Category = Animation)
		float GetMoveRight();
	UFUNCTION(BlueprintCallable, Category = Animation)
		float GetMoveForward();

	/** controls if the keyboard responds to user input **/
	UFUNCTION(BlueprintCallable, Category = Animation)
	void SetIsKeyboardEnabled(bool Enabled);

	/** returns the current attack the player is performing **/
	UFUNCTION(BlueprintCallable, Category = Animation)
	EAttackType GetCurrentAttack();

	UFUNCTION()
	void ResetCombo();

	UFUNCTION()
	void TriggerCountdownToIdle();

	UFUNCTION()
	void ArmPlayer(bool Value);

	UFUNCTION()
	void ArmPlayerImmediately();

	UFUNCTION()
	void RunStart();
	UFUNCTION()
	void RunEnd();

	UFUNCTION()
	void CrouchStart();

	UFUNCTION()
	void CrouchEnd();

	UFUNCTION()
	void Interact();

	void FireLineTrace();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = Trace)
	ELineTraceType LineTraceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trace)
	float LineTraceDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trace)
	float LineTraceSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trace)
	int32 MaxCountdownToIdle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
		float MaxCrouchSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
		float MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
		float MaxRunSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
		float MaxArmedSpeed;

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

	UAudioComponent* PunchThrowAudioComponent;

	FTimerHandle ComboResetTimerHandle;

	bool bIsCurrentlyPossessed;
private:
	UAudioComponent* PunchAudioComponent;

	FPlayerAttackMontage* AttackMontage;

	FMeleeCollisionProfile MeleeCollisionProfile;

	EAttackType CurrentAttack;
	EAttackStrength CurrentAttackStrength;

	AController* SavedController;

	bool bIsAnimationBlended;

	bool bIsKeyboardEnabled;

	bool bIsAttackLight;
	bool bIsAttackStrong;

	bool bIsRunning;

	float MoveForwardValue;
	float MoveRightValue;

	bool bIsArmed;
	FTimerHandle ArmedToIdleTimerHandle;

	int32 CountdownToIdle;

	int32 CurrentCombo;

	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	*/
	void Log(ELogLevel LogLevel, FString Message);
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	* @param ELogOutput - All, Output Log or Screen
	*/
	void Log(ELogLevel LogLevel, FString Message, ELogOutput LogOutput);
};


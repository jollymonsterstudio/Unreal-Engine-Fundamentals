// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"

#include "UE4Fundamentals06Character.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Engine.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	IsInAir = false;
	IsAnimationBlended = true;
	Speed = 0.f;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// cache the pawn
	Pawn = TryGetPawnOwner();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	//Always Check Pointers
	if (!Pawn)
	{
		return;
	}

	if (Pawn->IsA(AUE4Fundamentals06Character::StaticClass()))
	{
		AUE4Fundamentals06Character* PlayerCharacter = Cast<AUE4Fundamentals06Character>(Pawn);
		if (PlayerCharacter)
		{
			IsInAir = PlayerCharacter->GetMovementComponent()->IsFalling();
			IsAnimationBlended = PlayerCharacter->GetIsAnimationBlended();
			Speed = PlayerCharacter->GetVelocity().Size();

			GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow, "IsInAir: " + FString(IsInAir ? "true" : "false"));
			GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow, "IsAnimationBlended: " + FString(IsAnimationBlended ? "true" : "false"));
			GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow, "Speed: " + FString::SanitizeFloat(Speed));
		}
	}
}
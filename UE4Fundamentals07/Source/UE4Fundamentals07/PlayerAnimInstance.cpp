// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"

#include "UE4Fundamentals07Character.h"
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
	Owner = TryGetPawnOwner();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	// double check our pointers make sure nothing is empty
	if (!Owner)
	{
		return;
	}

	if (Owner->IsA(AUE4Fundamentals07Character::StaticClass()))
	{
		AUE4Fundamentals07Character* PlayerCharacter = Cast<AUE4Fundamentals07Character>(Owner);
		// again check pointers
		if (PlayerCharacter)
		{
			IsInAir = PlayerCharacter->GetMovementComponent()->IsFalling();
			IsAnimationBlended = PlayerCharacter->GetIsAnimationBlended();
			Speed = PlayerCharacter->GetVelocity().Size();

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "IsInAir: " + FString(IsInAir ? "true" : "false"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "IsAnimationBlended: " + FString(IsAnimationBlended ? "true" : "false"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Speed: " + FString::SanitizeFloat(Speed));
		}
	}
}
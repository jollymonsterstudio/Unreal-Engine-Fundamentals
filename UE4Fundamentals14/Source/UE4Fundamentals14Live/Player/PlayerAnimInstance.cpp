// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

#include "UE4Fundamentals14Live/UE4Fundamentals14LiveCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = TryGetPawnOwner();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!Owner)
	{
		return;
	}

	if(Owner->IsA(AUE4Fundamentals14LiveCharacter::StaticClass()))
	{
		AUE4Fundamentals14LiveCharacter* Character = Cast<AUE4Fundamentals14LiveCharacter>(Owner);

		bIsInAir = Character->GetMovementComponent()->IsFalling();
		Speed = Character->GetVelocity().Size();

		bIsFloating = Character->GetIsFloating();
		FloatAngle = Character->GetActorRotation().Pitch;
	}
}


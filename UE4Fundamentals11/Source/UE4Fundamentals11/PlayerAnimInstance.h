// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE4FUNDAMENTALS11_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bIsAnimationBlended;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bIsArmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bIsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float MoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float MoveForward;

public:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

private:
	APawn* Owner;
};

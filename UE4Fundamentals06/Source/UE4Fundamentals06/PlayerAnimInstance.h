// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UE4FUNDAMENTALS06_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool IsAnimationBlended;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Speed;

public:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

private:
	APawn* Pawn;
};

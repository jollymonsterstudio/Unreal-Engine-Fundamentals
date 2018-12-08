// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackStartNotifyState.h"

#include "UE4Fundamentals02Character.h"

#include "Engine.h"

void UAttackStartNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Magenta, __FUNCTION__);

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AUE4Fundamentals02Character* Player = Cast<AUE4Fundamentals02Character>(MeshComp->GetOwner());
		if (Player != NULL)
		{
			Player->AttackStart();
		}
	}
}

void UAttackStartNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Magenta, __FUNCTION__);

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AUE4Fundamentals02Character* Player = Cast<AUE4Fundamentals02Character>(MeshComp->GetOwner());
		if (Player != NULL)
		{
			Player->AttackEnd();
		}
	}
}
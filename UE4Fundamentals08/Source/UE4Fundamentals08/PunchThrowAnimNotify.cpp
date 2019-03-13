// Fill out your copyright notice in the Description page of Project Settings.

#include "PunchThrowAnimNotify.h"

#include "UE4Fundamentals08Character.h"

#include "Engine.h"

void UPunchThrowAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Purple, __FUNCTION__);

	/*if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		AUE4Fundamentals08Character* Player = Cast<AUE4Fundamentals08Character>(MeshComp->GetOwner());
		if (Player != NULL && !Player->PunchThrowAudioComponent->IsPlaying())
		{
			Player->PunchThrowAudioComponent->Play(0.f);
		}
	}*/
}
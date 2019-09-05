// Fill out your copyright notice in the Description page of Project Settings.

#include "ToggleProp.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"

#include "Components/StaticMeshComponent.h"

AToggleProp::AToggleProp()
{
	bCanScale = false;
}

void AToggleProp::Interact_Implementation()
{
	// iterate over all of our actors
	for(TActorIterator<AActor> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
	{
		AActor* Actor = *ActorIterator;
		// ensure actor is not null
		// ignore self if found
		// ensure we find actors of a specific interface only
		if(Actor && Actor != this && Actor->GetClass()->ImplementsInterface(UInteractiveActor::StaticClass()))
		{
			// display all available tags for an actor
			// for(FName Tag : Actor->Tags)
			// {
			// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Tag.ToString());
			// }

			for(FName TagToCheck : ToggleTags)
			{
				if(Actor->ActorHasTag(TagToCheck))
				{
					IInteractiveActor::Execute_Interact(Actor);		
				}
			}

			// if(Actor->ActorHasTag(ToggleTag))
			// {
			// 	IInteractiveActor::Execute_Interact(Actor);
			// }
		}
	}
}

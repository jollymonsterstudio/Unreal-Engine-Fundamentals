// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/InteractiveProp.h"
#include "ToggleProp.generated.h"

/**
 * 
 */
UCLASS()
class UE4FUNDAMENTALS12_API AToggleProp : public AInteractiveProp
{
	GENERATED_BODY()

public:
	AToggleProp();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> ToggleTags;

	virtual void Interact_Implementation() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovingSplineActor.h"
#include "GameFramework/Actor.h"
#include "UE4Fundamentals14Live/UE4Fundamentals14LiveCharacter.h"
#include "PlayerMovingSplineActor.generated.h"

UCLASS()
class UE4FUNDAMENTALS14LIVE_API APlayerMovingSplineActor : public AMovingSplineActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMovingSplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessMovementTimeline(float Value) override;

	virtual void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void OnEndMovementTimeline() override;

private:
	void DetachCharacter();

private:
	AUE4Fundamentals14LiveCharacter* Character;  
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovingSplineActor.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleMovingSplineActor.generated.h"

UCLASS()
class UE4FUNDAMENTALS14LIVE_API AParticleMovingSplineActor : public AMovingSplineActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleMovingSplineActor();

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta=(AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComponent;
};

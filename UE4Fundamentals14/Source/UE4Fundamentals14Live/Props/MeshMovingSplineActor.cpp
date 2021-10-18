// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshMovingSplineActor.h"

// Sets default values
AMeshMovingSplineActor::AMeshMovingSplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(SplineComponent);

	TriggerComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AMeshMovingSplineActor::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->SetWorldLocation(StartingSplineLocation);
}

// Called every frame
void AMeshMovingSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeshMovingSplineActor::ProcessMovementTimeline(float Value)
{
	Super::ProcessMovementTimeline(Value);

	FRotator SplineRotation = CurrentSplineRotation;
	SplineRotation.Pitch = 0.f;
	MeshComponent->SetWorldLocationAndRotation(CurrentSplineLocation, SplineRotation);
}

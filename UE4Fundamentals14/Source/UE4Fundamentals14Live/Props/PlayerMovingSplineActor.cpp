// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovingSplineActor.h"

// Sets default values
APlayerMovingSplineActor::APlayerMovingSplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerMovingSplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerMovingSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerMovingSplineActor::ProcessMovementTimeline(float Value)
{
	Super::ProcessMovementTimeline(Value);

	if(Character)
	{
		FRotator SplineRotation = CurrentSplineRotation;
		SplineRotation.Yaw = 0.f;

		Character->SetActorLocationAndRotation(CurrentSplineLocation, SplineRotation);
		TriggerComponent->SetWorldLocation(CurrentSplineLocation);
	}
}

void APlayerMovingSplineActor::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::TriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor->IsA(AUE4Fundamentals14LiveCharacter::StaticClass()))
	{
		Character = Cast<AUE4Fundamentals14LiveCharacter>(OtherActor);

		Character->SetInteractingActor(this);
		Character->SetIsFloating(true);
	}
}

void APlayerMovingSplineActor::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::TriggerEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	OnEndMovementTimeline();
}

void APlayerMovingSplineActor::OnEndMovementTimeline()
{
	Super::OnEndMovementTimeline();

	DetachCharacter();
}

void APlayerMovingSplineActor::DetachCharacter()
{
	if(Character)
	{
		Character->SetIsFloating(false);
		Character->SetInteractingActor(nullptr);
		Character = nullptr;

		TriggerComponent->SetWorldLocation(StartingSplineLocation);
	}
}

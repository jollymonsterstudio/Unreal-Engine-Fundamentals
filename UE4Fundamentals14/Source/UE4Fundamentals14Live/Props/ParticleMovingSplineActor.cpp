// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleMovingSplineActor.h"

// Sets default values
AParticleMovingSplineActor::AParticleMovingSplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	ParticleComponent->SetupAttachment(SplineComponent);
	ParticleComponent->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AParticleMovingSplineActor::BeginPlay()
{
	Super::BeginPlay();

	ParticleComponent->SetWorldLocation(StartingSplineLocation);
	ParticleComponent->SetActive(bAutoActivate);
}

// Called every frame
void AParticleMovingSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParticleMovingSplineActor::ProcessMovementTimeline(float Value)
{
	Super::ProcessMovementTimeline(Value);

	ParticleComponent->SetWorldLocation(CurrentSplineLocation);
}

void AParticleMovingSplineActor::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::TriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(!ParticleComponent->IsActive())
	{
		ParticleComponent->SetActive(true);
	}
}

void AParticleMovingSplineActor::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::TriggerEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if(ParticleComponent->IsActive())
	{
		ParticleComponent->SetActive(false);
	}
}

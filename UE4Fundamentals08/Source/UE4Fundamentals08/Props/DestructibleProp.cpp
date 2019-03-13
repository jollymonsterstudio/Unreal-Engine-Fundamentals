// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleProp.h"

#include "Engine.h"

// Sets default values
ADestructibleProp::ADestructibleProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destrcutible Component"));
	DestructibleComponent->SetupAttachment(RootComponent);
	DestructibleComponent->SetNotifyRigidBodyCollision(true);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Component"));
	TriggerComponent->SetupAttachment(RootComponent);

	IsDestroyed = false;
	IsTriggerEnabled = false;

	MaxHealth = 10.f;

	DefaultDamage = 1.f;
	DefaultImpulse = 1.f;

	TriggerCountdown = 10;
}

// Called when the game starts or when spawned
void ADestructibleProp::BeginPlay()
{
	Super::BeginPlay();

	DestructibleComponent->OnComponentHit.AddDynamic(this, &ADestructibleProp::Damage);

	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestructibleProp::Trigger);

	CurrentHealth = MaxHealth;
}

// Called every frame
void ADestructibleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestructibleProp::Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsDestroyed && OtherComp->ComponentHasTag("Weapon"))
	{		
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "Hit By: " + OtherComp->GetName());

		CurrentHealth = CurrentHealth - 1.f;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "Current Health: " + FString::SanitizeFloat(CurrentHealth));

		if (CurrentHealth <= 0.f)
		{
			Destroy(DefaultDamage, Hit.Location, NormalImpulse, DefaultImpulse);
		}
	}
}

void ADestructibleProp::Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsDestroyed && IsTriggerEnabled && OtherActor->ActorHasTag("Player"))
	{
		// make sure it's not active
		if (!GetWorld()->GetTimerManager().IsTimerActive(TriggerDestroyTimerHandle)) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "Triggered an explosion");
			GetWorld()->GetTimerManager().SetTimer(TriggerDestroyTimerHandle, this, &ADestructibleProp::TriggerCountdownToDestroy, 1.f, true);
		}
		else {
			// resetting timer
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "Pause Trigger");			
			GetWorld()->GetTimerManager().ClearTimer(TriggerDestroyTimerHandle);
		}
		
	}
}

void ADestructibleProp::TriggerDestroy()
{
	if (!IsDestroyed && IsTriggerEnabled)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "Explosion Triggered !!!!!");
		Destroy(DefaultDamage, DestructibleComponent->GetComponentLocation(), DestructibleComponent->GetUpVector(), DefaultImpulse);
	}
}

void ADestructibleProp::TriggerCountdownToDestroy()
{
	// count down to zero
	if (--TriggerCountdown <= 0) {
		int32 RandomDelay = FMath::RandRange(1, 5);

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "Boom ! with a delay of: " + FString::FromInt(RandomDelay));
		GetWorld()->GetTimerManager().SetTimer(TriggerDestroyTimerHandle, this, &ADestructibleProp::TriggerDestroy, RandomDelay, true);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "Counting down to explosion: " + FString::FromInt(TriggerCountdown));
	}
}

void ADestructibleProp::Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, __FUNCTION__);

	if (!IsDestroyed)
	{
		IsDestroyed = true;

		DestructibleComponent->ApplyDamage(Damage, HitLocation, ImpulseDir, Impulse);
	}
}


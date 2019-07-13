// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DestructibleComponent.h"

#include "../InteractiveActor.h"

#include "DestructibleProp.generated.h"

UCLASS()
class UE4FUNDAMENTALS10_API ADestructibleProp : public AActor, public IInteractiveActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleProp();

	UFUNCTION()
	void Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerDestroy();
	UFUNCTION()
		void TriggerCountdownToDestroy();

	UFUNCTION()
	void Destroy(float Damage, FVector HitLocation, FVector ImpulseDir, float Impulse);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
	class UBoxComponent* TriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destructible)
	class UDestructibleComponent* DestructibleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
	bool IsTriggerEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
	bool IsDestroyed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
	float DefaultDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Destructible)
	float DefaultImpulse;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void Interact();	// prototype declaration
	virtual void Interact_Implementation() override;	// actual implementation of our interact method


	FTimerHandle TriggerDestroyTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32 TriggerCountdown;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MovingSplineActor.generated.h"

UCLASS()
class UE4FUNDAMENTALS14LIVE_API AMovingSplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingSplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void ProcessMovementTimeline(float Value);

	UFUNCTION()
	virtual void OnEndMovementTimeline();

	UFUNCTION()
	virtual void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta=(AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta=(AllowPrivateAccess = "true"))
	UBoxComponent* TriggerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
	bool bAutoActivate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", meta=(EditCondition = "!bRestartOnEndTimeline"))
	bool bReverseOnEndTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", meta=(EditCondition = "!bReverseOnEndTimeline"))
	bool bRestartOnEndTimeline;

protected:
	FVector StartingSplineLocation;
	FVector CurrentSplineLocation;
	FRotator CurrentSplineRotation;

private:
	FTimeline MovementTimeline;
};

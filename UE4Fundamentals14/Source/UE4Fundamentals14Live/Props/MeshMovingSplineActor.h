// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovingSplineActor.h"
#include "GameFramework/Actor.h"
#include "MeshMovingSplineActor.generated.h"

UCLASS()
class UE4FUNDAMENTALS14LIVE_API AMeshMovingSplineActor : public AMovingSplineActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshMovingSplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ProcessMovementTimeline(float Value) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
};

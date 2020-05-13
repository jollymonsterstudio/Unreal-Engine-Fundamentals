// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "PhysicsProp.generated.h"

UCLASS()
class UE4FUNDAMENTALS12_API APhysicsProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsProp();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physics")
	class UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physics")
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physics")
	class UStaticMeshComponent* MovableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physics")
	class USkeletalMeshComponent* MovableSkeletalMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

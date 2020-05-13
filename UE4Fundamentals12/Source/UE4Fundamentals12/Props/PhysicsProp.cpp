// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsProp.h"

// Sets default values
APhysicsProp::APhysicsProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MovableMesh"));
	MovableMesh->SetupAttachment(RootComponent);
	MovableMesh->SetSimulatePhysics(true);
	MovableMesh->SetCollisionProfileName("Enemy");

	MovableSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MovableSkeletalMesh"));
	MovableSkeletalMesh->SetupAttachment(RootComponent);
	MovableSkeletalMesh->SetSimulatePhysics(true);
	MovableSkeletalMesh->SetCollisionProfileName("Enemy");

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APhysicsProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicsProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


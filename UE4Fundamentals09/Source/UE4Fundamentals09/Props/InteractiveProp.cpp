// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveProp.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AInteractiveProp::AInteractiveProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void AInteractiveProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveProp::Interact_Implementation()
{
	if(!bIsBig)
	{
		BaseMesh->SetWorldScale3D(FVector(2.f, 2.f, 2.f));		
	}
	else
	{
		BaseMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	}

	bIsBig = !bIsBig;
}



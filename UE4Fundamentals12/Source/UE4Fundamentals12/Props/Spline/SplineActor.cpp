// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"

#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if(SplineComponent)
	{
		SetRootComponent(SplineComponent);	
	}	
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplineActor::OnConstruction(const FTransform& Transform)
{	
	Super::OnConstruction(Transform);

	if(SplineComponent && SplineMeshMap.Num() > 0)
	{
		// lookup all pertinent values
		FSplineMeshDetails* StartMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::START))
		{
			StartMeshDetails = SplineMeshMap.Find(ESplineMeshType::START);	
		}

		FSplineMeshDetails* EndMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::END))
		{
			EndMeshDetails = SplineMeshMap.Find(ESplineMeshType::END);
		}

		FSplineMeshDetails* DefaultMeshDetails = nullptr;
		if(SplineMeshMap.Contains(ESplineMeshType::DEFAULT))
		{
			DefaultMeshDetails = SplineMeshMap.Find(ESplineMeshType::DEFAULT);	
		}
		else
		{
			// exit if we don't have a default mesh to work with
			return;
		}
		

		for(int SplineCount = 0; SplineCount < (SplineComponent->GetNumberOfSplinePoints() - 1); SplineCount++)
		{
			USplineMeshComponent *SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	
			const int32 SplinePoints = SplineComponent->GetNumberOfSplinePoints();


			
	
			// first
			if(StartMeshDetails && EndMeshDetails)
			{
				if(SplineCount == 0)
				{
					SplineMesh->SetStaticMesh(StartMeshDetails->Mesh);
					SplineMesh->SetForwardAxis(StartMeshDetails->ForwardAxis, true);
					
					if(StartMeshDetails->DefaultMaterial)
					{
						SplineMesh->SetMaterial(0, StartMeshDetails->DefaultMaterial);	
					}					
				}
				else if(SplinePoints > 2 && SplineCount == (SplinePoints - 2))
				{
					// end cap
					SplineMesh->SetStaticMesh(EndMeshDetails->Mesh);
					SplineMesh->SetForwardAxis(EndMeshDetails->ForwardAxis, true);
					
					if(EndMeshDetails->DefaultMaterial)
					{
						SplineMesh->SetMaterial(0, EndMeshDetails->DefaultMaterial);	
					}					
				}
				else
				{
					// assign our mesh
					SplineMesh->SetStaticMesh(DefaultMeshDetails->Mesh);
					SplineMesh->SetForwardAxis(DefaultMeshDetails->ForwardAxis, true);
				
					if(DefaultMeshDetails->EvenMaterial && SplineCount > 0 && SplineCount % 2 == 0)
					{
						SplineMesh->SetMaterial(0, DefaultMeshDetails->EvenMaterial);
					}
					else if(DefaultMeshDetails->DefaultMaterial)
					{
						SplineMesh->SetMaterial(0, DefaultMeshDetails->DefaultMaterial);
					}			
				}
			}
			else
			{
				// assign our mesh
				SplineMesh->SetStaticMesh(DefaultMeshDetails->Mesh);
				SplineMesh->SetForwardAxis(DefaultMeshDetails->ForwardAxis, true);
				
				if(DefaultMeshDetails->EvenMaterial && SplineCount > 0 && SplineCount % 2 == 1)
				{
					SplineMesh->SetMaterial(0, DefaultMeshDetails->EvenMaterial);
				}
				else if(DefaultMeshDetails->DefaultMaterial)
				{
					SplineMesh->SetMaterial(0, DefaultMeshDetails->DefaultMaterial);
				}				
			}
	
			// initialize the object
			SplineMesh->RegisterComponentWithWorld(GetWorld());
	
			SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			SplineMesh->SetMobility(EComponentMobility::Movable);
	
			SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
			// define the positions of the points and tangents
			const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Type::Local);
			const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Type::Local);
			const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Type::Local);
			const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Type::Local);
			SplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
	
			// query physics
			SplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		}
	}
}

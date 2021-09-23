// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"

#include "../UE4Fundamentals13Character.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("MovementPath");
	SplineComponent->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SplineComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Trigger");
	TriggerBox->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();

	FRotator NewRotation = SplineComponent->GetRotationAtTime(0.f, ESplineCoordinateSpace::World);
	NewRotation.Pitch = 0.f;
	Mesh->SetWorldRotation(NewRotation);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASplineActor::OnTriggerBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ASplineActor::OnTriggerEndOverlap);

	if(bMovePlayerInsteadOfMesh)
	{
		Mesh->SetHiddenInGame(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}
}


void ASplineActor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, FString::Printf(TEXT("OtherActor: %s"), *OtherActor->GetName()));
	// only do something if we are the player
	if(OtherActor->IsA(AUE4Fundamentals13Character::StaticClass()))
	{		
		TriggerMove();

		if(bMovePlayerInsteadOfMesh)
		{
			OverlappingActor = OtherActor;

			// AUE4Fundamentals13Character* PlayerCharacter = Cast<AUE4Fundamentals13Character>(OtherActor);
			// PlayerCharacter->SetIsFloating(true);			
		}
	}
}

void ASplineActor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	if(!MovementTimeline.IsReversing() && !bMovePlayerInsteadOfMesh)
	{
		MovementTimeline.Reverse();
	}
}


void ASplineActor::MoveAlongTimeline(float Value)
{
	const float SplineLength = SplineComponent->GetSplineLength();
	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, FString::Printf(TEXT("Splinelength: %f"), Value * SplineLength));

	CurrentLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);
	CurrentRotation = SplineComponent->GetRotationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);

	if(bMovePlayerInsteadOfMesh && OverlappingActor)
	{
		OverlappingActor->SetActorLocationAndRotation(CurrentLocation, CurrentRotation);
	}
	else
	{
		// kill the pitch in order to prevent weird rotations
		CurrentRotation.Pitch = 0.f;
		Mesh->SetWorldLocationAndRotation(CurrentLocation, CurrentRotation);	
	}	
}

void ASplineActor::TriggerMove()
{
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, TEXT("MoveAlongTimeline"));

	if(!MovementTimeline.IsPlaying())
	{
		MovementTimeline = FTimeline();
		MovementTimeline.AddInterpFloat(SplineCurve, ProgressFunction);
		MovementTimeline.SetLooping(false);
		MovementTimeline.PlayFromStart();		
	}
	else
	{
		MovementTimeline.Play();
	}
	
	//Set the timeline's length to match the last key frame based on the given curve
	MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);

	//The ResetActiveFlightStopActor executes when the timeline finishes.
	//By calling ResetActiveFlightStopActor at the end of the timeline we make sure to reset any invalid references on ActiveFlightStopActor
	FOnTimelineEvent TimelineEvent;
	TimelineEvent.BindUFunction(this, FName("StopMovement"));
	MovementTimeline.SetTimelineFinishedFunc(TimelineEvent);	
}

void ASplineActor::StopMovement()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "Function: StopMovement");	
	MovementTimeline.Stop();

	if(OverlappingActor)
	{
		// AUE4Fundamentals13Character* PlayerCharacter = Cast<AUE4Fundamentals13Character>(OverlappingActor);
		// PlayerCharacter->SetIsFloating(false);

		CurrentRotation.Pitch = 0.f;
		
		OverlappingActor->SetActorRotation(CurrentRotation);
		OverlappingActor = nullptr;
	}	
}

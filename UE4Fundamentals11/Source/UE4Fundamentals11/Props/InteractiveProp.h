// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../InteractiveActor.h"

#include "InteractiveProp.generated.h"

UCLASS()
class UE4FUNDAMENTALS11_API AInteractiveProp : public AActor, public IInteractiveActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveProp();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanScale;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void Interact();	// prototype declaration
	virtual void Interact_Implementation() override;	// actual implementation of our interact method


private:
	bool bIsBig;
};

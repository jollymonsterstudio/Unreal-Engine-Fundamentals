// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4Fundamentals13GameMode.h"
#include "UE4Fundamentals13Character.h"
#include "UObject/ConstructorHelpers.h"

AUE4Fundamentals13GameMode::AUE4Fundamentals13GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4Fundamentals14LiveGameMode.h"
#include "UE4Fundamentals14LiveCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE4Fundamentals14LiveGameMode::AUE4Fundamentals14LiveGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

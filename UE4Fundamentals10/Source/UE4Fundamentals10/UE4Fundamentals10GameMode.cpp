// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UE4Fundamentals10GameMode.h"
#include "UE4Fundamentals10Character.h"
#include "UObject/ConstructorHelpers.h"

AUE4Fundamentals10GameMode::AUE4Fundamentals10GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

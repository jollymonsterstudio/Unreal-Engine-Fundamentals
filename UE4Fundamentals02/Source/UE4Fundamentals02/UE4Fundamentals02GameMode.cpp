// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UE4Fundamentals02GameMode.h"
#include "UE4Fundamentals02Character.h"
#include "UObject/ConstructorHelpers.h"

AUE4Fundamentals02GameMode::AUE4Fundamentals02GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

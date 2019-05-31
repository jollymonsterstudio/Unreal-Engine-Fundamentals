// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UE4Fundamentals09GameMode.h"
#include "UE4Fundamentals09Character.h"
#include "UObject/ConstructorHelpers.h"

AUE4Fundamentals09GameMode::AUE4Fundamentals09GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

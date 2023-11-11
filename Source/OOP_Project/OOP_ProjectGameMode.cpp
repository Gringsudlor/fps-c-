// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OOP_ProjectGameMode.h"
#include "OOP_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOOP_ProjectGameMode::AOOP_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

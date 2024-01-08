// Copyright Epic Games, Inc. All Rights Reserved.

#include "KJY_TProjectGameMode.h"
#include "KJY_TProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKJY_TProjectGameMode::AKJY_TProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Copyright 2020 Yevhenii Selivanov

#include "GameplayFramework/GASGameMode.h"
//---
#include "GASCharacter.h"
#include "GAS/Public/GASPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AGASGameMode::AGASGameMode()
{
	// set default classes
	DefaultPawnClass = AGASCharacter::StaticClass();
	PlayerControllerClass = AGASPlayerController::StaticClass();
}

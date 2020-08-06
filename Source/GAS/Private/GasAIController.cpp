// Copyright 2020 Yevhenii Selivanov

#include "GasAIController.h"

#include "BrainComponent.h"

//
void AGasAIController::SetLogicState(bool bShouldEnable) const
{
	if (!ensureMsgf(BrainComponent, TEXT("BrainComponent is invalid")))
	{
		return;
	}

	if (bShouldEnable)
	{
		BrainComponent->StartLogic();
	}
	else
	{
		BrainComponent->StopLogic(FString(TEXT("Stopped")));
	}
}

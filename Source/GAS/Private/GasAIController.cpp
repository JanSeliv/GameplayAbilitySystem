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
		BrainComponent->ResumeLogic(FString(TEXT("Resumed")));
	}
	else
	{
		BrainComponent->PauseLogic(FString(TEXT("Paused")));
	}
}

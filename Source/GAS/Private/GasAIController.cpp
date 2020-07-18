// Copyright 2020 Yevhenii Selivanov

#include "GasAIController.h"

#include "BrainComponent.h"

//
void AGasAIController::StopLogic() const
{
	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("Dead"));
	}
}

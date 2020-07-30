// Copyright 2020 Yevhenii Selivanov

#include "GASPlayerController.h"
#include "Abilities/GameplayAbilityBase.h"

void AGASPlayerController::AddAbilityToUI_Implementation(TSubclassOf<UGameplayAbilityBase> AbilityBaseClass)
{
	// BP implementation
}

void AGASPlayerController::OnAbilityPressed_Implementation(int32 AbilityN)
{
	// BP implementation
}

// Allows the PlayerController to set up custom input bindings.
void AGASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	DECLARE_DELEGATE_OneParam(FOnAbilityActivated, int32);
	for (int i = 1; i <= 5; ++i)
	{
		const FName ActionName{FString("Ability_0") + FString::FromInt(i)};
		InputComponent->BindAction<FOnAbilityActivated>(ActionName, EInputEvent::IE_Pressed, this, &ThisClass::OnAbilityPressed, i).
		                bConsumeInput = false;
	}
}

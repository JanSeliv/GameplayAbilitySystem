// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "GameFramework/PlayerController.h"
#include "GASPlayerController.generated.h"

/**
 *
 */
UCLASS()
class GAS_API AGASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 *
	 * @param InAbilityBaseClass
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++")
	void AddAbilityToUI(TSubclassOf<class UGameplayAbilityBase> InAbilityBaseClass);
};

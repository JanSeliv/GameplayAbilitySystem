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

protected:
	/**  */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
	void OnAbilityPressed(int32 AbilityN);

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;
};

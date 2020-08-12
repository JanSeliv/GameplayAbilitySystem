// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "Abilities/GameplayAbilityTargetActor.h"
#include "GameplayAbilityTargetActor_GroundSelect.generated.h"

/**
 *
 */
UCLASS()
class GAS_API AGameplayAbilityTargetActor_GroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	/** Requesting targeting data, but not necessarily stopping/destroying the task. Useful for external target data requests. */
	virtual void ConfirmTargetingAndContinue() override;

	/** Initialize and begin targeting logic  */
	virtual void StartTargeting(UGameplayAbility* Ability) override;

protected:
	/**  */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void TraceGround();

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float StartLength = 10000.0F; //[N]
};

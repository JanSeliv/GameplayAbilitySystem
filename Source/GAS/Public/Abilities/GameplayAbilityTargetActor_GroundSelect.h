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
public:

	GENERATED_BODY()

public:
	/** Default constructor */
	AGameplayAbilityTargetActor_GroundSelect();

	/** Requesting targeting data, but not necessarily stopping/destroying the task.
	  * Is called by ConfirmTarget (f.e by Wait Target Data event). */
	virtual void ConfirmTargetingAndContinue() override;

	/** Initialize and begin targeting logic  */
	virtual void StartTargeting(UGameplayAbility* Ability) override;

protected:
	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++",  meta = (ExposeOnSpawn = true, BlueprintProtected))
	float StartLength = 10000.0F; //[N

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++",  meta = (ExposeOnSpawn = true, BlueprintProtected))
	float Radius = 100.0F; //[N

	/** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	FGameplayAbilityTargetDataHandle TargetData; //[B]

#if WITH_EDITOR
	/** Function called every frame on this Actor */
	virtual void Tick(float DeltaSeconds) override;
#endif

	/** */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	bool TraceGround();
};

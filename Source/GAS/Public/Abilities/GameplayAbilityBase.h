// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EAbilityCostType : uint8
{
	None,
    Health,
    Mana,
    Strength
};

/**
 *
 */
UCLASS(Abstract)
class GAS_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    static float GetCooldownDurationByClass(const TSubclassOf<UGameplayAbilityBase>& ForAbilityClass);

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	float GetCooldownDuration() const;

	/**
	* @param ForAbilityClass
	* @param OutCostValue Returns Cost Value
	* @return Returns Cost type or EAbilityCostType::None.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    static EAbilityCostType GetCostModifierByClass(const TSubclassOf<UGameplayAbilityBase>& ForAbilityClass, float& OutCostValue);

	/**
	 * @param OutCostValue Returns Cost Value
	 * @return Returns Cost type or EAbilityCostType::None.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    EAbilityCostType GetCostModifier(float& OutCostValue) const;

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    FORCEINLINE class UMaterialInstance* GetUIMaterial() const { return UIMaterial; }

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    FORCEINLINE class UAnimMontage* GetMontage() const { return AnimMontage; }

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    float GetMontageLength() const;

protected:
	/** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	class UMaterialInstance* UIMaterial; //[B]

	/** */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	class UAnimMontage* AnimMontage; //[B]

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};

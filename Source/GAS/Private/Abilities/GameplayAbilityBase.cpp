// Copyright 2020 Yevhenii Selivanov

#include "Abilities/GameplayAbilityBase.h"

#include "Abilities/AttributeSetBase.h"
#include "Animation/AnimMontage.h"

float UGameplayAbilityBase::GetCooldownDuration() const
{
	float CooldownDuration = 0.F;
	UGameplayEffect* const CooldownEffect = GetCooldownGameplayEffect();
	if (ensureMsgf(CooldownEffect, TEXT("ASSERT: 'CooldownEffect' condition is FALSE")))
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1.F, CooldownDuration);
	}

	return CooldownDuration;
}

EAbilityCostType UGameplayAbilityBase::GetCostModifier(float& OutCostValue) const
{
	UGameplayEffect* const CostEffect = GetCostGameplayEffect();
	if (ensureMsgf(CostEffect && CostEffect->Modifiers.IsValidIndex(0),
		TEXT("ASSERT: 'CostEffect && CostEffect->Modifiers.IsValidIndex(0)' condition is FALSE")))
	{
		// Set Cost
		const FGameplayModifierInfo& EffectInfo = CostEffect->Modifiers[0];
		EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.F, OutCostValue);

		// return CostType
		if (const static UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAbilityCostType"), true))
		{
			return static_cast<EAbilityCostType>(Enum->GetValueByNameString(EffectInfo.Attribute.AttributeName,
				EGetByNameFlags::ErrorIfNotFound));
		}
	}

	return EAbilityCostType::None;
}

float UGameplayAbilityBase::GetMontageLength() const
{
	return AnimMontage ? AnimMontage->GetPlayLength() : 0.F;
}

// Actually activate ability, do not call this directly.
void UGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ensureMsgf(UIMaterial && AnimMontage, TEXT("ASSERT: 'UIMaterial && AnimMontage' condition is FALSE"));
}

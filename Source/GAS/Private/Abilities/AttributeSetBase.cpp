// Copyright 2020 Yevhenii Selivanov

#include "Abilities/AttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

#define MAX_HEALTH_VALUE 200.F

// Default constructor
UAttributeSetBase::UAttributeSetBase()
	: Health(MAX_HEALTH_VALUE)
	, MaxHealth(MAX_HEALTH_VALUE)
{
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	auto DataProperty = Data.EvaluatedData.Attribute.GetUProperty();
	if (DataProperty
	    && DataProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)
		&& OnHealthChange.IsBound())
	{
		const float NewHealthVal = FMath::Clamp(Health.GetCurrentValue(), 0.F, MaxHealth.GetCurrentValue());
		Health.SetCurrentValue(NewHealthVal);
		const float NewPercentage = NewHealthVal / MaxHealth.GetCurrentValue();

		OnHealthChange.Broadcast(NewPercentage);
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffect: %f"), NewPercentage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffect: FAILED"));
	}
}

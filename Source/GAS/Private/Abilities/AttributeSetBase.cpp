// Copyright 2020 Yevhenii Selivanov

#include "Abilities/AttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

// Default constructor
UAttributeSetBase::UAttributeSetBase() : Health(200.F)
{
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	auto DataProperty = Data.EvaluatedData.Attribute.GetUProperty();
	if (DataProperty
		&& DataProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health))
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffect: %f"), Health.GetCurrentValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffect: FAILED"));
	}
}

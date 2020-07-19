// Copyright 2020 Yevhenii Selivanov

#include "Abilities/AttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

void FAttributeStructBase::SetCurrentValue(float NewValue)
{
	const float NewHealthValue = FMath::Clamp(NewValue, 0.F, GetMaxValue());
	Super::SetCurrentValue(NewHealthValue);

	const float NewPercentage = NewHealthValue / GetMaxValue();
	OnAttributeChange.Broadcast(NewPercentage);
	UE_LOG(LogTemp, Warning, TEXT("SetCurrentValue: %f"), NewPercentage);
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// const FProperty* DataProperty = Data.EvaluatedData.Attribute.GetUProperty();
	// if(!DataProperty)
	// {
	// 	return;
	// }

	// if (DataProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health))
	// {
	//
	// }
}

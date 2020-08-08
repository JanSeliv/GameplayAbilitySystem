// Copyright 2020 Yevhenii Selivanov

#include "Abilities/AttributeSetBase.h"

#include "GASCharacter.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

// Modifies current value, normally only called by ability system or during initialization.
void FGASGameplayAttributeData::SetCurrentValue(float NewValue)
{
	const float NewHealthValue = FMath::Clamp(NewValue, 0.F, GetMaxValue());
	Super::SetCurrentValue(NewHealthValue);

	const float NewPercentage = NewHealthValue / GetMaxValue();
	OnAttributeChanged.Broadcast(NewPercentage);

	UE_LOG(LogTemp, Warning, TEXT("SetCurrentValue: %f"), NewPercentage);
}

//
bool FGASGameplayAttributeData::IsMaxValue() const
{
	return FMath::IsNearlyEqual(GetCurrentValue(), GetMaxValue(), KINDA_SMALL_NUMBER);
}

bool FGASGameplayAttributeData::IsPositiveValue() const
{
	return GetCurrentValue() > 0.F;
}

// 	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FProperty* DataProperty = Data.EvaluatedData.Attribute.GetUProperty();
	if (!DataProperty)
	{
		return;
	}

	if (DataProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health))
	{
		if (const auto OwnedGASCharacter = Cast<AGASCharacter>(GetOwningActor()))
		{
			if (Health.IsMaxValue())
			{
				OwnedGASCharacter->AddGameplayTag(Health.GetMaxValueTag());
			}
			else
			{
				OwnedGASCharacter->RemoveGameplayTag((Health.GetMaxValueTag()));
			}
		}
	}
}

// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "AttributeSet.h"
#include "GameplayTagContainer.h"

#include "AttributeSetBase.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FAttributeStructBase : public FGameplayAttributeData
{
	GENERATED_BODY()

	FAttributeStructBase() : Super(), MaxValue(0.F) {}

	FAttributeStructBase(float Value) : Super(Value), MaxValue(Value) {}

	/** */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float);
	FOnHealthChange OnAttributeChange;

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float MaxValue;

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	FGameplayTag MaxValueTag;

	/** */
	FORCEINLINE float GetMaxValue() const { return MaxValue; }

	/** */
	FORCEINLINE FGameplayTag GetMaxValueTag() const { return MaxValueTag; }

	/** Modifies current value, normally only called by ability system or during initialization. */
	virtual void SetCurrentValue(float NewValue) override;

	/** */
	bool IsMaxValue() const;
};

/**
 *
 */
UCLASS()
class UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UAttributeSetBase(){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FAttributeStructBase Health = 200.F;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FAttributeStructBase Mana = 150.F;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FAttributeStructBase Strength = 250.F;

protected:
	/**
	 *	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called
	 *	during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};

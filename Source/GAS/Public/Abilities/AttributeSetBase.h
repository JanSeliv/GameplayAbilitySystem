// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "AttributeSet.h"
#include "GameplayAbilityBase.h"
#include "GameplayTagContainer.h"

#include "AttributeSetBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float);

/**
 *
 */
USTRUCT(BlueprintType)
struct FGASGameplayAttributeData : public FGameplayAttributeData
{
	GENERATED_BODY()

	FGASGameplayAttributeData() : Super(), MaxValue(0.F) {}

	FGASGameplayAttributeData(float Value) : Super(Value), MaxValue(Value) {}

	/** */
	FOnAttributeChanged OnAttributeChanged;

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

	/**  */
	bool IsPositiveValue() const;
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

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGASGameplayAttributeData Health = 200.F;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGASGameplayAttributeData Mana = 150.F;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGASGameplayAttributeData Strength = 250.F;

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    static FORCEINLINE bool IsMaxAttributeValue(const FGASGameplayAttributeData& Attribute) { return Attribute.IsMaxValue(); }

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    static FORCEINLINE bool IsPositiveAttributeValue(const FGASGameplayAttributeData& Attribute) { return Attribute.IsPositiveValue(); }

	/**
	 *	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called
	 *	during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};

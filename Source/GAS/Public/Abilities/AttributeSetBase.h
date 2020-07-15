// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "AttributeSet.h"

#include "AttributeSetBase.generated.h"

/**
 *
 */
UCLASS()
class UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UAttributeSetBase();

	/** This measures how much damage can be absorbed before dying. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData Health;

protected:
	/**
	 *	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called
	 *during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};


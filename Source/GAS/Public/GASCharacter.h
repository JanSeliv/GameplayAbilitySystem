// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

#include "GASCharacter.generated.h"

UCLASS(config = Game)
class AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASCharacter();

	// Base
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	UFUNCTION(BlueprintCallable, Category = "C++")
	class USpringArmComponent* GetCameraBoom() const;

	/** Returns FollowCamera subobject **/
	UFUNCTION(BlueprintCallable, Category = "C++")
	class UCameraComponent* GetFollowCamera() const;

	/** Returns the ability system component to use for this actor.
	 *  It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (BlueprintProtected))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (BlueprintProtected))
	class UCameraComponent* FollowCamera;

	/** Our ability system */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (BlueprintProtected))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	class UAttributeSetBase* AttributeSetBase;


	/** Called for forwards/backward input */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void MoveForward(float Value);

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void LookUpAtRate(float Rate);

	/** */
	UFUNCTION(BlueprintCallable, Category = "C++")
	void AcquireAbility(TSubclassOf<class UGameplayAbility> AbilityToAcquire);

	/** Overridable native event for when play begins for this actor. */
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHealthChange(float NewPercentage);

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void DieCharacter();

	/** */
	uint8 bIsDead:1;
};

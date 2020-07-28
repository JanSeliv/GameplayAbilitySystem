// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
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
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	class USpringArmComponent* GetCameraBoom() const;

	/** Returns FollowCamera subobject **/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	class UCameraComponent* GetFollowCamera() const;

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    FORCEINLINE bool IsAlive() const { return !bIsDead; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	FORCEINLINE bool IsInputEnabled() const { return !bIsInputDisabled; }

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	bool IsEnemyCharacter(const ACharacter* CharacterToCheck) const;

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	uint8 GetTeamID() const { return TeamID; }

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
    void AddGameplayTag(const FGameplayTag& TagToAdd);

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
    void RemoveGameplayTag(const FGameplayTag& TagToRemove);

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
    bool CanUseAbilities() const;

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	void GetAcquiredAbilities(TArray<TSubclassOf<class UGameplayAbilityBase>>& OutAcquiredAbilities) const;

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

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	class UAttributeSetBase* AttributeSetBase;

	/** @see Getter: AGASCharacter::GetAcquiredAbilities() */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	TArray<TSubclassOf<class UGameplayAbilityBase>> AcquiredAbilities; //[M.DO]

	/** Called for forwards/backward input */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void MoveForward(float Value);

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void MoveRight(float Value);

	/** */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void AutoDetermineTeamIDByControllerType();

	/**
	 * @see Getter: AGASCharacter::GetTeamID()
	 * @see Setted: AGASCharacter::AutoDetermineTeamIDByControllerType()
	 */
	uint8 TeamID = 255;

	/**
	* @see Getter: AGASCharacter::IsAlive()
	* @see Setter: AGASCharacter::DieCharacter()
	*/
	uint8 bIsDead:1;

	/**
	* @see Getter: AGASCharacter::IsInputEnabled()
	* @see Setter AGASCharacter::SetInputControl()
	*/
	uint8 bIsInputDisabled:1;

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
	void AcquireAbilities(const TArray<TSubclassOf<class UGameplayAbilityBase>>& Abilities);

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
    void OnHealthChanged(float NewPercentage);

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
    void OnManaChanged(float NewPercentage);

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
    void OnStrengthChanged(float NewPercentage);

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
    void DieCharacter();

	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
    void SetInputControl(bool bShouldEnable);

	/**
	 * @brief
	 * @param InAbilityBaseClass
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++", meta = (BlueprintProtected))
	void AddAbilityToUI(TSubclassOf<class UGameplayAbilityBase> InAbilityBaseClass);

	/** Make the character jump on the next update. */
	virtual void Jump() override;

	/** Overridable native event for when play begins for this actor. */
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
};

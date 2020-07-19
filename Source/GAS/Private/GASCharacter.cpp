// Copyright 2020 Yevhenii Selivanov

#include "GASCharacter.h"

#include "Abilities/AttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "GasAIController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

//////////////////////////////////////////////////////////////////////////
// AGASCharacter

AGASCharacter::AGASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;            // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;       // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Initialize the ability system components
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSetBase"));
}

USpringArmComponent* AGASCharacter::GetCameraBoom() const
{
	return CameraBoom;
}

UCameraComponent* AGASCharacter::GetFollowCamera() const
{
	return FollowCamera;
}

bool AGASCharacter::IsEnemyCharacter(const AGASCharacter* CharacterToCheck) const
{
	return CharacterToCheck && CharacterToCheck->TeamID != TeamID;
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGASCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGASCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGASCharacter::AutoDetermineTeamIDByControllerType()
{
	if (Cast<APlayerController>(GetController()))
	{
		TeamID = 0;
	}
}

void AGASCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGASCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGASCharacter::AcquireAbility(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	if (!HasAuthority()
	    || !AbilitySystemComponent
	    || !Abilities.Num())
	{
		return;
	}

	// Grants abilities
	for (const TSubclassOf<UGameplayAbility>& AbilityIt : Abilities)
	{
		if (AbilityIt)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityIt));
		}
	}

	// Initialize the structure that holds information about who we are acting on and who controls us.
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AGASCharacter::OnHealthChanged_Implementation(float NewPercentage)
{
	if (NewPercentage <= 0.F //
	    && !bIsDead)
	{
		bIsDead = 1;
		DieCharacter();
	}

	// BP implementation
}

void AGASCharacter::OnManaChanged_Implementation(float NewPercentage)
{
	// BP implementation
}

void AGASCharacter::OnStrengthChanged_Implementation(float NewPercentage)
{
	// BP implementation
}

void AGASCharacter::DieCharacter_Implementation()
{
	AController* const OwnedController = GetController();
	if (OwnedController)
	{
		if (const auto PlayerController = Cast<APlayerController>(OwnedController))
		{
			DisableInput(PlayerController);
		}
		else if (const auto GasAIController = Cast<AGasAIController>(OwnedController))
		{
			GasAIController->StopLogic();
		}
	}

	// BP implementation
}

// Overridable native event for when play begins for this actor.
void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(AttributeSetBase, TEXT("AttributeSetBase is invalid")))
	{
		AttributeSetBase->Health.OnAttributeChange.AddUObject(this, &ThisClass::OnHealthChanged);
		AttributeSetBase->Mana.OnAttributeChange.AddUObject(this, &ThisClass::OnManaChanged);
		AttributeSetBase->Strength.OnAttributeChange.AddUObject(this, &ThisClass::OnStrengthChanged);
	}

	AutoDetermineTeamIDByControllerType();
}

void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGASCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGASCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGASCharacter::LookUpAtRate);
}

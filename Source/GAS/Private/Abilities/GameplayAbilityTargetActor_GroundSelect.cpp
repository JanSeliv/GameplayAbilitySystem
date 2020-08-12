// Copyright 2020 Yevhenii Selivanov


#include "Abilities/GameplayAbilityTargetActor_GroundSelect.h"

#include "Abilities/GameplayAbility.h"

void AGameplayAbilityTargetActor_GroundSelect::ConfirmTargetingAndContinue()
{
	// Super call was removed to avoid broadcasting empty FGameplayAbilityTargetDataHandle() param

	TraceGround();
}

// Initialize and begin targeting logic
void AGameplayAbilityTargetActor_GroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	if (!ensureMsgf(Ability, TEXT("ASSERT: 'Ability' condition is FALSE")))
	{
		return;
	}

	const AActor* OwningActor = Ability->GetOwningActorFromActorInfo();
	if (OwningActor)
	{
		MasterPC = Cast<APlayerController>(OwningActor->GetInstigatorController());
	}
}

void AGameplayAbilityTargetActor_GroundSelect::TraceGround()
{
	UWorld* World = GetWorld();
	APawn* MasterPawn = MasterPC ? MasterPC->GetPawn() : nullptr;
	if (!World
	    || !MasterPawn)
	{
		return;
	}

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(MasterPawn);

	FVector StartVector;
	FRotator ViewRotation;
	MasterPC->GetPlayerViewPoint(StartVector, ViewRotation);
	const FVector EndVector = StartVector + ViewRotation.Vector() * StartLength;

	FHitResult HitResult;
	bool bIsSuccessTrace = World->LineTraceSingleByChannel(HitResult, StartVector, EndVector, ECC_Visibility, QueryParams);

	FVector LookAtPoint = FVector::ZeroVector;
	if(bIsSuccessTrace)
	{
		LookAtPoint = HitResult.ImpactPoint;
	}
}

// Copyright 2020 Yevhenii Selivanov


#include "Abilities/GameplayAbilityTargetActor_GroundSelect.h"

#include "Abilities/GameplayAbility.h"

// Requesting targeting data, but not necessarily stopping/destroying the task. Useful for external target data requests.
void AGameplayAbilityTargetActor_GroundSelect::ConfirmTargetingAndContinue()
{
	// Super call was removed to avoid broadcasting empty FGameplayAbilityTargetDataHandle() param
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

bool AGameplayAbilityTargetActor_GroundSelect::TraceGround(FVector& OutViewLocation)
{
	const UWorld* World = GetWorld();
	const APawn* MasterPawn = MasterPC ? MasterPC->GetPawn() : nullptr;
	if (!World
	    || !MasterPawn)
	{
		return false;
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
	if (!bIsSuccessTrace)
	{
		return false;
	}
	OutViewLocation = HitResult.ImpactPoint;

	FVector ViewLocation;
	TraceGround(ViewLocation);

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	const bool bTraceComplex = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = bTraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	CollisionQueryParams.AddIgnoredActor(MasterPawn);

	const bool bSuccessOverlap =
		World->OverlapMultiByChannel(Overlaps, ViewLocation, FQuat::Identity,
			ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(StartLength), CollisionQueryParams);
	if (!bSuccessOverlap)
	{
		return false;
	}

	for (const FOverlapResult& OverlapIt : Overlaps)
	{
		auto PawnOverlapped = Cast<APawn>(OverlapIt.GetActor());
		if (PawnOverlapped
		    && !OverlappedActors.Contains(PawnOverlapped))
		{
			OverlappedActors.Add(PawnOverlapped);
		}
	}

	return bIsSuccessTrace;
}

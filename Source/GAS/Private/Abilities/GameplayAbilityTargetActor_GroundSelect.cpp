// Copyright 2020 Yevhenii Selivanov


#include "Abilities/GameplayAbilityTargetActor_GroundSelect.h"
#include "DrawDebugHelpers.h"
#include "Abilities/GameplayAbility.h"

// Requesting targeting data, but not necessarily stopping/destroying the task. Useful for external target data requests.
void AGameplayAbilityTargetActor_GroundSelect::ConfirmTargetingAndContinue()
{
	// Super call was removed to avoid broadcasting empty FGameplayAbilityTargetDataHandle() param
	if (!IsConfirmTargetingAllowed())
	{
		return;
	}

	const bool bSucceededTraceGround = TraceGround();
	if (bSucceededTraceGround)
	{
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		Super::ConfirmTargetingAndContinue();
	}
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

// Function called every frame on this Actor
void AGameplayAbilityTargetActor_GroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if WITH_EDITOR
	if(bDebug)
	{
		TraceGround();
	}
#endif
}

//
bool AGameplayAbilityTargetActor_GroundSelect::TraceGround()
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
	StartVector = HitResult.ImpactPoint;

	if(bDebug)
	{
		DrawDebugSphere(World, StartVector, Radius, 32, FColor::Red, true, World->DeltaTimeSeconds*10.F, 0, 5.F);
	}

	TArray<FOverlapResult> Overlaps;
	const bool bTraceComplex = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = bTraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	CollisionQueryParams.AddIgnoredActor(MasterPawn);

	const bool bSuccessOverlap =
		World->OverlapMultiByChannel(Overlaps, StartVector, FQuat::Identity,
			ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(StartLength), CollisionQueryParams);

	if (!bSuccessOverlap)
	{
		return false;
	}

	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	for (const FOverlapResult& OverlapIt : Overlaps)
	{
		auto PawnOverlapped = Cast<APawn>(OverlapIt.GetActor());
		if (PawnOverlapped
		    && !OverlappedActors.Contains(PawnOverlapped))
		{
			OverlappedActors.Add(PawnOverlapped);
		}
	}

	if (!OverlappedActors.Num())
	{
		return false;
	}

	TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
	return true;
}

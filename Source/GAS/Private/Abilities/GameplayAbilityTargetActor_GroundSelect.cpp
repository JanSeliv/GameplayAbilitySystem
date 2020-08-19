// Copyright 2020 Yevhenii Selivanov


#include "Abilities/GameplayAbilityTargetActor_GroundSelect.h"
#include "DrawDebugHelpers.h"
#include "Abilities/GameplayAbility.h"

#define TICK_INTERVAL 0.25F

AGameplayAbilityTargetActor_GroundSelect::AGameplayAbilityTargetActor_GroundSelect()
{
#if WITH_EDITOR
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bStartWithTickEnabled = true;
		PrimaryActorTick.TickInterval = TICK_INTERVAL;
#endif
}

// Requesting targeting data, but not necessarily stopping/destroying the task. Is called by ConfirmTarget (f.e by Wait Target Data event)
void AGameplayAbilityTargetActor_GroundSelect::ConfirmTargetingAndContinue()
{
	// Super call was removed to avoid broadcasting empty FGameplayAbilityTargetDataHandle() param
	if (!IsConfirmTargetingAllowed())
	{
		return;
	}

	// Execute broadcast to call to finish Waiting the Target Data event
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

#if WITH_EDITOR
// Function called every frame on this Actor
void AGameplayAbilityTargetActor_GroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bDebug)
	{
		TraceGround();
	}
}
#endif

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
		DrawDebugSphere(World, StartVector, Radius, 32, FColor::Red, false, TICK_INTERVAL, 0, 5.F);
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

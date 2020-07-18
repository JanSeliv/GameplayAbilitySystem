// Copyright 2020 Yevhenii Selivanov

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GasAIController.generated.h"

/**
 *
 */
UCLASS()
class GAS_API AGasAIController : public AAIController
{
	GENERATED_BODY()

public:
	/** Default constructor */
	AGasAIController(){}

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "C++")
	void StopLogic() const;
};

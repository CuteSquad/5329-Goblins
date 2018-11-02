// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GoblinAnimInstance.generated.h"

/**
 * Animation Instance class for all goblins
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class FPSPROJECT_API UGoblinAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "States")
	bool bCanWalk = false;
	
	
};

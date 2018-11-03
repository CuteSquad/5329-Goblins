// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Stun.generated.h"

/**
 * Play goblin stun animation if IsHit is true.
 */
UCLASS()
class FPSPROJECT_API UBTTask_Stun : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

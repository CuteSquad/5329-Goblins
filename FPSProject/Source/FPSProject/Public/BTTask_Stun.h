// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GoblinCharacter.h"
#include "FirstPersonCharacter.h"
#include "BTTask_Stun.generated.h"

/**
 * Play goblin stun animation if IsHit is true.
 */
UCLASS()
class FPSPROJECT_API UBTTask_Stun : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Stun();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	AFirstPersonCharacter* PlayerCharacter = nullptr;
	AGoblinCharacter* GoblinCharacter = nullptr;
};

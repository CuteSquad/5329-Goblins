// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetWalkSpeed.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBTTask_SetWalkSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 300.0f;
};

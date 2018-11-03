// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GoblinCharacter.h"
#include "GoblinAIController.h"
#include "GruntGoblin.h"
#include "BTTask_MoveToPlayer.generated.h"

/**
 * Moves the AI Pawn to the specified Player in the world
 */
UCLASS()
class FPSPROJECT_API UBTTask_MoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	AGoblinCharacter * GoblinCharacter = nullptr;
	AGruntGoblin * GruntGoblin = nullptr;
};

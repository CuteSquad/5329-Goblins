// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GoblinCharacter.h"
#include "GoblinAIController.generated.h"

/**
 * Base AI Controller Class for all goblins
 */
UCLASS()
class FPSPROJECT_API AGoblinAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGoblinAIController();

	virtual void Possess(APawn* InPawn) override;

	uint8 KeyID;

private:
	UPROPERTY(transient) // transient disables serialization
		UBlackboardComponent* BlackBoardComponent;

	UPROPERTY(transient) // transient disables serialization
		UBehaviorTreeComponent* BehaviourTreeComponent;
};

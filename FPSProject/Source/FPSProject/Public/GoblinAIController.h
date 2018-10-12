// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
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

private:
	UPROPERTY(transient)
		UBlackboardComponent* BlackBoardComponent;

	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviourTreeComponent;

	uint8 KeyID;
};

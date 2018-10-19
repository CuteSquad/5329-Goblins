// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GoblinAIController.h"
#include "GoblinCharacter.h"
#include "Engine/Engine.h"
#include "BTService_SetRandomLocation.generated.h"

/**
 * Sets a random location for the AI to move to in the background
 */
UCLASS()
class FPSPROJECT_API UBTService_SetRandomLocation : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SetRandomLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	UPROPERTY(EditInstanceOnly, Category = "Service")
	FBlackboardKeySelector BlackboardKey;
	
};

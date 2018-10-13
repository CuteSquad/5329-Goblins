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
#include "BTService_CheckPlayer.generated.h"

/**
 * Background checks for player
 */
UCLASS()
class FPSPROJECT_API UBTService_CheckPlayer : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPlayer();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
};

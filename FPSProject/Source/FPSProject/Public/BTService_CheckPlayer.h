// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GoblinAIController.h"
#include "Engine/UserDefinedEnum.h"
#include "Engine/TargetPoint.h"
#include "BTService_CheckPlayer.generated.h"

/**
 * A check for the Goblin AI to see the player
 */
UCLASS()
class FPSPROJECT_API UBTService_CheckPlayer : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPlayer();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector StateKey;

	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector GoalActorKey;
	
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector MemoryMarkerKey;

	UPROPERTY(EditAnywhere, Category = "Sight")
	float SightInDegrees = 55.0f;

	UPROPERTY(EditAnywhere, Category = "Sight")
	bool bDrawDebugLineToPlayer = false;

	UPROPERTY(EditAnywhere, Category = "Memory")
	bool bDrawDebugMemoryMarker = false;

private:
	ATargetPoint* TargetPoint = nullptr;

	UUserDefinedEnum* GoblinAIState = nullptr;
	
	FHitResult HitResult;
};

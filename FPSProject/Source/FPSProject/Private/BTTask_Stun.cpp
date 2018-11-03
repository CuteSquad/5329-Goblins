// Copyright © 2018, AJACKS

#include "BTTask_Stun.h"
#include "GoblinAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GoblinCharacter.h"


EBTNodeResult::Type UBTTask_Stun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	const auto GoblinCharacter = Cast<AGoblinCharacter>(GoblinController->GetPawn());

	GoblinCharacter->GoblinAnimInstance->bIsStunned = true;

	return EBTNodeResult::Succeeded;
}


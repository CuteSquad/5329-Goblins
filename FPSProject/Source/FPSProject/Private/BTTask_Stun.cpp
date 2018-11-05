// Copyright © 2018, AJACKS

#include "BTTask_Stun.h"
#include "GoblinAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UBTTask_Stun::UBTTask_Stun()
{
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Stun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	GoblinCharacter = Cast<AGoblinCharacter>(GoblinController->GetPawn());

	if (GoblinCharacter->bIsGoblinHit)
	{
		GoblinCharacter->GoblinAnimInstance->bIsStunned = true; // Play stun anim

		return EBTNodeResult::Succeeded;
	}
	else
	{
		GoblinCharacter->GoblinAnimInstance->bIsStunned = false; // Don't play stun anim
		GoblinCharacter->bIsGoblinHit = false;
		GoblinCharacter->GoblinAnimInstance->bCanWalk = true;

		return EBTNodeResult::Failed;
	}
}

void UBTTask_Stun::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	GoblinCharacter->GoblinAnimInstance->bIsStunned = false;
	GoblinCharacter->bIsGoblinHit = false;
	GoblinCharacter->GoblinAnimInstance->bCanWalk = true;
}

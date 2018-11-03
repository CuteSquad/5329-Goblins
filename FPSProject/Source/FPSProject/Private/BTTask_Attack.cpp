// Copyright © 2018, AJACKS

#include "BTTask_Attack.h"
#include "GoblinAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GoblinCharacter.h"


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	APawn *Goblin = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(FName("SelfActor")));
	APawn *Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->KeyID[0]));

	const auto GoblinCharacter = Cast<AGoblinCharacter>(GoblinController->GetPawn());

	if (Player && GoblinController->AIStimulus.WasSuccessfullySensed())
	{
		GoblinCharacter->GoblinAnimInstance->bCanAttack = true;
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}


// Copyright © 2018, AJACKS

#include "BTTask_MoveToPlayer.h"


EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	APawn *Goblin = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->KeyID[1]));
	APawn *Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->KeyID[0]));

	if (Player)
	{
		GoblinController->MoveToActor(Player, 5.0f, true, true, true, 0, true);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}



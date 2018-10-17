// Copyright © 2018, AJACKS

#include "BTTask_MoveToPlayer.h"


EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	APawn *Goblin = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->SelfID));
	APawn *Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->TargetID));
	FVector RandomLocation = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(GoblinController->VectorID);

	if (Player)
	{
		GoblinController->MoveToActor(Player, 5.0f, true, true, true, 0, true);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		GoblinController->MoveToLocation(RandomLocation, -1, true, true, false, true, nullptr, true);
		return EBTNodeResult::Failed;
	}
}



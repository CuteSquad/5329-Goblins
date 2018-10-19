// Copyright © 2018, AJACKS

#include "BTTask_MoveToRandomLocation.h"


EBTNodeResult::Type UBTTask_MoveToRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	APawn *Goblin = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->KeyID[1]));
	const FVector RandomLocation = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	
	if (Goblin)
	{
		GoblinController->MoveToLocation(RandomLocation, 500, true, true, true, true, nullptr, true);
		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;

}


// Copyright © 2018, AJACKS

#include "BTTask_MoveToPlayer.h"


EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	APawn *Goblin = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(FName("SelfActor")));
	APawn *Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->KeyID[0]));

	const auto GoblinCharacter = Cast<AGoblinCharacter>(Goblin);

	if (Player && GoblinController->AIStimulus.WasSuccessfullySensed() && !GoblinCharacter->GoblinAnimInstance->bHealthBelowZero)
	{
		if (!GoblinCharacter->GoblinAnimInstance->bHealthBelowZero)
		{
			GoblinController->MoveToActor(Player, 50.0f, true, true, true, nullptr, true);
			GoblinCharacter->GoblinAnimInstance->bCanAttack = true;
		}
		else
		{
			GoblinCharacter->GoblinAnimInstance->bCanWalk = false;
			GoblinCharacter->Destroy();
		}
		

		return EBTNodeResult::Succeeded;
	}
	else
	{
		if (GoblinCharacter->GoblinAnimInstance->bHealthBelowZero)
			GoblinCharacter->Destroy();

		return EBTNodeResult::Failed;
	}
}



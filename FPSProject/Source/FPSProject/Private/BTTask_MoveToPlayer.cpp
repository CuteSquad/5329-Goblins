// Copyright © 2018, AJACKS

#include "BTTask_MoveToPlayer.h"


EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());
	//APawn *Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->KeyID[0]));
	//
	//const auto GoblinCharacter = Cast<AGoblinCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	//
	//if (Player && GoblinController->AIStimulus.WasSuccessfullySensed() && !GoblinCharacter->GoblinAnimInstance->bHealthBelowZero)
	//{
	//	if (PathResult != EPathFollowingRequestResult::Type::RequestSuccessful)
	//	{
	//		PathResult = GoblinController->MoveToActor(Player, 50.0f, true, true, true, nullptr, true);
	//		return EBTNodeResult::InProgress;
	//	}
	//
	//	PathResult = GoblinController->MoveToActor(Player, 50.0f, true, true, true, nullptr, true);
	//
	//	return EBTNodeResult::Succeeded;
	//}
	//
	//if (GoblinCharacter->GoblinAnimInstance->bHealthBelowZero)
	//{
	//	GoblinCharacter->Destroy();
	//	GoblinController->Destroy();
	//}
	//
	//
	//if (GoblinCharacter->bIsGoblinHit)
	//{
	//	GoblinController->MoveToActor(Player, 50.0f, true, true, true, nullptr, true);
	//}
	//
	return EBTNodeResult::Failed;
}


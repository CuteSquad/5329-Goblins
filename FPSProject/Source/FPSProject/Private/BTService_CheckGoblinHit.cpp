// Copyright © 2018, AJACKS

#include "BTService_CheckGoblinHit.h"
#include "FirstPersonCharacter.h"


UBTService_CheckGoblinHit::UBTService_CheckGoblinHit()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckGoblinHit::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Get the AI Controller class
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	if (GoblinController)
	{
		// Get the gobin in the world possessed by their AI Controllers
		APawn *Goblin = OwnerComp.GetAIOwner()->GetPawn();
		const auto GoblinCharacter = Cast<AGoblinCharacter>(Goblin);

		if (Goblin && GoblinCharacter->bIsGoblinHit)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsHit"), true);
		}
		else if (Goblin && !GoblinCharacter->bIsGoblinHit)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("bIsHit"), false);
		}
	}
}

void UBTService_CheckGoblinHit::OnGameplayTaskActivated(UGameplayTask& Task)
{

}

void UBTService_CheckGoblinHit::OnGameplayTaskDeactivated(UGameplayTask& Task)
{

}



// Copyright © 2018, AJACKS

#include "BTService_CheckPlayer.h"


UBTService_CheckPlayer::UBTService_CheckPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Get the AI Controller class
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	if (GoblinController)
	{
		// Get the player in the world
		APawn *Player = GetWorld()->GetFirstPlayerController()->GetPawn();

		if (Player)
		{
			// And set the target object in the blackboard to the player pawn
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(GoblinController->KeyID, Player);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Goblin is here!");
		}
	}
}

void UBTService_CheckPlayer::OnGameplayTaskActivated(UGameplayTask& Task)
{
	
}

void UBTService_CheckPlayer::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	
}


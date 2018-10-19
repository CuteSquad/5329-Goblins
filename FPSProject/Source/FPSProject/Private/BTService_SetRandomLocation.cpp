// Copyright © 2018, AJACKS

#include "BTService_SetRandomLocation.h"


UBTService_SetRandomLocation::UBTService_SetRandomLocation()
{
	bCreateNodeInstance = true;
	bNotifyBecomeRelevant = true;
}


void UBTService_SetRandomLocation::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{

}

void UBTService_SetRandomLocation::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());

	if (GoblinController)
	{
		// Get the player in the world
		APawn *Goblin = OwnerComp.GetAIOwner()->GetPawn();

		if (Goblin)
		{
			FVector RandomLocation = Goblin->GetActorLocation();
			RandomLocation.X = FMath::FRandRange(0, Goblin->GetActorLocation().X + 100);
			RandomLocation.Y = FMath::FRandRange(0, Goblin->GetActorLocation().Y + 100);
			RandomLocation.Z = 0;

			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(GoblinController->KeyID[2], RandomLocation);
		}
	}	
}

void UBTService_SetRandomLocation::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void UBTService_SetRandomLocation::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}

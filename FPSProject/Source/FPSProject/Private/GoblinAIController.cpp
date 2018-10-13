// Copyright © 2018, AJACKS

#include "GoblinAIController.h"


AGoblinAIController::AGoblinAIController()
{
	// Blackboard Component
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(FName("Blackboard Component"));

	// Behaviour Tree Component
	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("Behaviour Tree Component"));
}

void AGoblinAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AGoblinCharacter* GoblinCharacter = Cast<AGoblinCharacter>(InPawn);

	if (GoblinCharacter && GoblinCharacter->GoblinBehavior)
	{
		BlackBoardComponent->InitializeBlackboard(*GoblinCharacter->GoblinBehavior->BlackboardAsset);
		KeyID = BlackBoardComponent->GetKeyID("Target");
		BehaviourTreeComponent->StartTree(*GoblinCharacter->GoblinBehavior);
	}
}



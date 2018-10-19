// Copyright © 2018, AJACKS

#include "GoblinAIController.h"
#include "Perception/AIPerceptionComponent.h"


AGoblinAIController::AGoblinAIController()
{
	// Blackboard Component
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(FName("Blackboard Component"));

	// Behaviour Tree Component
	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("Behaviour Tree Component"));

	// Initialise Key Names
	KeyNames[0] = "Target";
	KeyNames[1] = "SelfActor";
	KeyNames[2] = "RandomLocation";
	KeyNames[3] = "";
	KeyNames[4] = "";

	// Set keys
	for (uint8 i = 0; i < MaxKeys; i++)
		KeyID[i] = BlackBoardComponent->GetKeyID(KeyNames[i]);

	// AI Perception default values
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(FName("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AGoblinAIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AGoblinAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AGoblinCharacter* GoblinCharacter = Cast<AGoblinCharacter>(InPawn);

	if (GoblinCharacter && GoblinCharacter->GoblinBehavior)
	{
		BlackBoardComponent->InitializeBlackboard(*GoblinCharacter->GoblinBehavior->BlackboardAsset);

		// Store blackboard keyIds for use in the Behaviour tree services
		for (uint8 i = 0; i < MaxKeys; i++)
			KeyID[i] = BlackBoardComponent->GetKeyID(KeyNames[i]);

		BehaviourTreeComponent->StartTree(*GoblinCharacter->GoblinBehavior);
	}
}

FRotator AGoblinAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(FRotator::ZeroRotator);
	}

	return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
}


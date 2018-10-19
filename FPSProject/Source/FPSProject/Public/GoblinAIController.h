// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GoblinCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GoblinAIController.generated.h"

/**
 * Base AI Controller Class for all goblins
 */
UCLASS()
class FPSPROJECT_API AGoblinAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGoblinAIController();

	virtual void BeginPlay() override;
	virtual void Possess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
		void OnPawnDetected(TArray<AActor*> DetectedPawns);

	UPROPERTY(VisibleAnywhere, Category = "AI")
		float AISightRadius = 500.0f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		float AILoseSightRadius = AISightRadius + 50.0f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		float AIFieldOfView = 90.0f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		UAISenseConfig_Sight *SightConfig;

	uint8 KeyID[5];

private:
	UPROPERTY(transient) // transient disables serialization
		UBlackboardComponent* BlackBoardComponent;

	UPROPERTY(transient) // transient disables serialization
		UBehaviorTreeComponent* BehaviourTreeComponent;

	int MaxKeys = 5;
	FName KeyNames[5];
};

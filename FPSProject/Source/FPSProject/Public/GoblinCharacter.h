// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/StaticMesh.h"
#include "GoblinAnimInstance.h"
#include "GoblinAIController.h"
#include "GoblinCharacter.generated.h"

/**
 * Base Goblin Character class for all goblins
 */
UCLASS(hidecategories = ("Actor", "Input", "HLOD"))
class FPSPROJECT_API AGoblinCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGoblinCharacter();

	UPROPERTY(EditAnywhere, Category = "Behaviour")
		UBehaviorTree* GoblinBehavior = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Health")
		int MaxHealth = 100;

	UPROPERTY(EditInstanceOnly, Category = "Health")
		int CurrentHealth = MaxHealth;

	UGoblinAnimInstance* GoblinAnimInstance = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	USkeletalMesh *SK_Goblin = nullptr;

	AGoblinAIController* GoblinAIController = nullptr;
};

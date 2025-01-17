// Copyright © 2018, AJACKS

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/StaticMesh.h"
#include "GoblinAnimInstance.h"
#include "GoblinAIController.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
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

	UPROPERTY(EditAnywhere, Category = "Goblin", BlueprintReadOnly)
		USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Behaviour")
		UBehaviorTree* GoblinBehavior = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Health")
		int MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
		int CurrentHealth = MaxHealth;

	bool bIsGoblinHit = false;
	bool bHasAttacked = false;
	
	UGoblinAnimInstance* GoblinAnimInstance = nullptr;

	void Dissolve();

	UFUNCTION()
	void OnDissolve();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	USkeletalMesh *SK_Goblin = nullptr;

	AGoblinAIController* GoblinAIController = nullptr;

	// Material References
	UMaterialInterface* GoblinBodyMaterial = nullptr;
	UMaterialInterface* GoblinEyeMaterial = nullptr;
	UMaterialInterface* GoblinTeethMaterial = nullptr;
	UMaterialInstanceDynamic* GoblinBodyMaterialInstance = nullptr;
	UMaterialInstanceDynamic* GoblinEyeMaterialInstance = nullptr;
	UMaterialInstanceDynamic* GoblinTeethMaterialInstance = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTimelineComponent* DissolveTimeline = nullptr;

	UCurveFloat* DissolveCurveFloat = nullptr;
};


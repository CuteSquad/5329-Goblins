// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GoblinCharacter.generated.h"

UCLASS(hidecategories = ("Actor", "Input", "HLOD"))
class FPSPROJECT_API AGoblinCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGoblinCharacter();

	UPROPERTY(EditAnywhere, Category = "Behaviour")
		UBehaviorTree* GoblinBehavior = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(EditInstanceOnly, Category = "Mesh")
		UStaticMeshComponent *StaticMeshComponent = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Mesh")
		UStaticMesh *Goblin = nullptr;
};

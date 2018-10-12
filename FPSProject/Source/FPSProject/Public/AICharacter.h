// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Classes/AIController.h"
#include "Kismet/GameplayStatics.h"
#include "AICharacter.generated.h"

UCLASS()
class FPSPROJECT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditInstanceOnly, Category = "Behaviour")
		UBehaviorTree* BotBehavior;
};

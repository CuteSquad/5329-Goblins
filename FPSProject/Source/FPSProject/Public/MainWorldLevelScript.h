// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "FirstPersonCharacter.h"
#include "MainWorldLevelScript.generated.h"

/**
 * The default class for the LevelBlueprint
 */
UCLASS()
class FPSPROJECT_API AMainWorldLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EnableInput(APlayerController* PlayerController) override;

	void OnPlayerDeath() const;
	void RestartLevel() const;
};

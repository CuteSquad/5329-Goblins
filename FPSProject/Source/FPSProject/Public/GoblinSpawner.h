// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GoblinSpawner.generated.h"

UCLASS(hidecategories = ("Rendering","Actor", "Input", "HLOD"))
class FPSPROJECT_API AGoblinSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoblinSpawner();

	UFUNCTION()
	void SpawnGoblin();

protected:
	virtual void BeginPlay() override;

private:
	USceneComponent *Root = nullptr;

	FTimerHandle Handle;

	UPROPERTY(EditInstanceOnly, Category = "Spawner Settings", meta = (ToolTip = "How long to wait before another goblin spawns?"))
	float SpawnDelay = 1.0f;

	UPROPERTY(EditInstanceOnly, Category = "Spawner Settings", meta = (ToolTip = "How many goblins to spawn?"))
	uint16 Goblins = 10;

	UPROPERTY(EditInstanceOnly, Category = "Spawner Settings", meta = (ToolTip = "What type of goblin to spawn?"))
	TSubclassOf<ACharacter> GoblinType;

	UPROPERTY(VisibleInstanceOnly, Category = "Debug", meta = (ToolTip = "Total Goblins in the world"))
	uint16 GoblinCount = 0;
};

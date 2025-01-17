// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoblinSpawner.h"
#include "GoblinCounter.generated.h"

UCLASS(hidecategories = ("Rendering", "Input", "Actor", "HLOD"))
class FPSPROJECT_API AGoblinCounter : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoblinCounter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, meta = (ToolTip = "Show how many goblins have spawned in the world"))
	bool bShowDebugMessage = false;

	UPROPERTY(VisibleAnywhere)
	TArray<AGoblinSpawner*> Spawners;

	UPROPERTY(VisibleAnywhere)
	uint16 NumOfGoblins = 0;
};

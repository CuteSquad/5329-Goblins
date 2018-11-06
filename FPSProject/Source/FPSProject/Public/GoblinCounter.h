// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoblinSpawner.h"
#include "GoblinCounter.generated.h"

UCLASS()
class FPSPROJECT_API AGoblinCounter : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoblinCounter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TArray<AGoblinSpawner*> Spawners;
	
};

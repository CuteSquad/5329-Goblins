// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "TeleportToLevel.generated.h"

UCLASS(hidecategories = ("Rendering","Actor", "Input", "HLOD"))
class FPSPROJECT_API ATeleportToLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	ATeleportToLevel();

protected:
	UFUNCTION()
	void FadeOutScreen(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void Teleport(TAssetPtr<UWorld> InLevel) const;

	UPROPERTY(EditInstanceOnly)
	UBoxComponent* TriggerBoxComponent = nullptr; // The trigger to teleport to level

	UPROPERTY(EditInstanceOnly, Category = "Level", meta = (ToolTip = "Level to teleport to when player overlaps with the trigger box"))
	TAssetPtr<UWorld> LevelToTeleportTo;

private:
	FTimerHandle TeleportDelayTimerHandle;
	FTimerDelegate TeleportDelayTimerDel;
};

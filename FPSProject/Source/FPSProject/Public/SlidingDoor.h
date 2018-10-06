// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "SlidingDoor.generated.h"

UCLASS(hidecategories = ("Input", "Actor", "HLOD"))
class FPSPROJECT_API ASlidingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASlidingDoor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Slide(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	USceneComponent* Root = nullptr;
	
	UPROPERTY(EditInstanceOnly)
		UStaticMeshComponent* StaticMeshComponent = nullptr;

	UStaticMesh* Door = nullptr;
	
	UPROPERTY(EditInstanceOnly)
		UBoxComponent* TriggerBoxComponent = nullptr;

	UPROPERTY(EditInstanceOnly)
		UTimelineComponent* Timeline = nullptr;

	UPROPERTY(EditInstanceOnly)
		UCurveFloat* CurveFloat = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Slide", meta = (ToolTip = "Should the door slide sideways?"))
		bool Sideways = false;

	UPROPERTY(EditInstanceOnly, Category = "Slide", meta = (ToolTip = "How far should the door slide on the X-Axis?", EditCondition = "Sideways"))
		float XSlide = 300;

	UPROPERTY(EditInstanceOnly, Category = "Slide", meta = (ToolTip = "How far should the door slide on the Y-Axis?", EditCondition = "Sideways"))
		float YSlide = 300;

	UPROPERTY(EditInstanceOnly, Category = "Slide", meta = (ToolTip = "How far should the door slide on the Z-Axis?", EditCondition = "!Sideways"))
		float ZSlide = 350;

	UPROPERTY(EditInstanceOnly, Category = "Slide", meta = (ToolTip = "How fast should the door slide? 0 = no movement, 1 = normal, 2+ = fast"))
		float Speed = 1;

	FVector CurrentLocation = FVector(0, 0, 0);
	FVector EndLocation = FVector(0, 0, 0);
};

// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "Ladder.generated.h"

UCLASS(hidecategories = ("Actor", "Input", "HLOD"))
class FPSPROJECT_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	ALadder();

protected:
	virtual void BeginPlay() override;

	// Turn on/off climbing mode
	UFUNCTION()
	void ToggleClimbOn(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void ToggleClimbOff(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void MoveForward(float Scale);
	virtual void MoveRight(float Scale);

private:
	USceneComponent* Root = nullptr;

	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* StaticMeshComponent = nullptr;
	UPROPERTY(EditInstanceOnly)
	UStaticMesh* Ladder = nullptr; // The ladder mesh

	UPROPERTY(EditInstanceOnly)
	UBoxComponent* TriggerBoxComponent = nullptr; // To trigger climbing functionality 

	APawn* OurPawn = nullptr;
	UCharacterMovementComponent* CharacterMovement = nullptr;

	bool CanClimb = false;
};

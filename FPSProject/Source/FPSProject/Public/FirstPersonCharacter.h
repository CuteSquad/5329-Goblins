// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "FirstPersonCharacter.generated.h"

UCLASS()
class FPSPROJECT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions
	void MoveXAxis(float AxisValue);
	void MoveYAxis(float AxisValue);

	void Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent *FirstPersonCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USkeletalMeshComponent* SK_Mesh = nullptr;

	USkeletalMesh* FP_Arms = nullptr; // FP = First Person

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* SK_Gun = nullptr;

	USkeletalMesh* FP_Gun = nullptr; // FP = First Person

	UTextRenderComponent* HealthCounter = nullptr;
	UTextRenderComponent* AmmoCounter = nullptr;
	UMaterialInstance* TextMaterial = nullptr;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ToolTip = "The max health of the player"))
		float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ToolTip = "The current health of the player"))
		float CurrentHealth = MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ToolTip = "The displayed health"))
		FString DisplayedHealth = FString("100%");

	UPROPERTY(EditDefaultsOnly, Category = "Ammo", meta = (ToolTip = "The max ammo that the play can have"))
		uint8 MaxAmmo = 15;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo", meta = (ToolTip = "The max ammo that the play can have"))
		uint8 CurrentAmmo = MaxAmmo;

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
};

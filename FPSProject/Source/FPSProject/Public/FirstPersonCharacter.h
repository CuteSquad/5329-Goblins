// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "FirstPersonCharacter.generated.h"

UCLASS(Blueprintable, BlueprintType)
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USphereComponent *Sphere = nullptr;

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

	UPROPERTY(EditDefaultsOnly, Category = "Ammo", meta = (ToolTip = "The current ammo that the play has"))
		uint8 CurrentAmmo = MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Firing", meta = (ToolTip = "How far should the line trace go"))
		float WeaponRange = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Firing", meta = (ToolTip = "How much damage should the player output"))
		float WeaponDamage = 500.0f;

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);

	UAnimMontage *FireMontage = nullptr;

	// Gun offset from the camera location
	FVector GunOffset = FVector(100.0f, 0.0f, -10.0f);
	FVector CameraLoc;
	FRotator CameraRot;
	FVector StartTrace, EndTrace, ShootDir;

	USoundBase *WeaponFire = nullptr;

	TSubclassOf<AActor> BlueprintItem;
	TSubclassOf<AActor> PlayerBlueprint;

	FHitResult BulletHitResult;
};

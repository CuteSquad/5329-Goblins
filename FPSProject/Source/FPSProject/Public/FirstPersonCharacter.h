// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
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

	UFUNCTION()
	void OnCalculateHealth(); // Timeline will run this every frame of the timeline

	void CalculateHealth();
	void CalculateAmmo();
	void Reload();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent *FirstPersonCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* SK_Mesh = nullptr;

	USkeletalMesh* FP_Arms = nullptr; // FP = First Person

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* SK_Gun = nullptr;

	USkeletalMesh* FP_Gun = nullptr; // FP = First Person

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent *Sphere = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextRenderComponent* HealthCounter = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextRenderComponent* AmmoCounter = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTimelineComponent* TimelineComponent = nullptr;

	UCurveFloat* CurveFloat = nullptr;
	
	UMaterialInstance* TextMaterial = nullptr;
	UMaterialInstanceDynamic* DiamondUIMaterialInstance = nullptr;
	TArray<UStaticMeshComponent*>::ElementType* HealthMonitor = nullptr;
	UMaterialParameterCollection* ScalarParams = nullptr;

	TArray<UStaticMeshComponent*> Components;

private:

	UPROPERTY(EditAnywhere, Category = "Health", meta = (ToolTip = "The max health of the player"))
		uint16 MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health", meta = (ToolTip = "The current health of the player"))
		uint16 CurrentHealth = MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Ammo", meta = (ToolTip = "The max ammo that the player can have"))
		uint16 MaxAmmo = 15;

	UPROPERTY(VisibleAnywhere, Category = "Ammo", meta = (ToolTip = "The current ammo that the player has"))
		uint16 CurrentAmmo = MaxAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Health", meta = (ToolTip = "The displayed health"))
		FText DisplayedHealth = FText::AsPercent(1.0f);

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "How far should the weapon range be?"))
		float WeaponRange = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "How much damage should the player output?"))
		float WeaponDamage = 500.0f;

	bool bCanShoot = true;
	float SphereTraceRadius = 50.0f;
	float LerpSpeed = 2.0f;

	uint16 NewHealth = 0;

	// Gun offset from the camera location
	FVector GunOffset = FVector(100.0f, 0.0f, -10.0f);
	FVector StartTrace, EndTrace;

	FAttachmentTransformRules AttachmentRuleSnap = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	FAttachmentTransformRules AttachmentRuleRelative = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

	FHitResult BulletHitResult;

	UAnimMontage *FireMontage = nullptr;
	USoundBase *WeaponFire = nullptr;

	TSubclassOf<AActor> BlueprintItem;
};

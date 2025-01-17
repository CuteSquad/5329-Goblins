// Copyright © 2018, AJACKS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "GoblinCharacter.h"
#include "GruntGoblin.h"
#include "HUDWidget.h"
#include "Particles/ParticleSystem.h"
#include "WidgetComponent.h"
#include "FirstPersonCharacter.generated.h"

class AMainWorldLevelScript;

UCLASS(Blueprintable, BlueprintType)
class FPSPROJECT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonCharacter();

	void AddHealth(int NewHealth);

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(int GoblinAttackDamage);

	// Editable variables in editor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (ToolTip = "The current health of the player"))
	int CurrentHealth = MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Player", meta = (ToolTip = "Number of Goblins the player has killed"))
	uint16 GoblinsKilled = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Player", meta = (ToolTip = "Number of Goblins the player has killed"))
	uint16 GoblinsKilledForDoorUnlock = 0;

	// Widget References
	UPROPERTY(BlueprintReadOnly)
	UHUDWidget* HUD = nullptr; // HealthPickup class needs this

	UPROPERTY(BlueprintReadOnly)
	UHUDWidget* Crosshair = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions
	void MoveXAxis(float AxisValue);
	void MoveYAxis(float AxisValue);

	void Fire();

	UFUNCTION()
	void OnCalculateHealth(); // Timeline will run this every tick of the timeline

	UFUNCTION()
	void OnCalculateAmmo();

	UFUNCTION()
	void OnReload();

	UFUNCTION()
	void SetDashState();

	UFUNCTION()
	void SetFireState();

	UFUNCTION()
	void RemoveHitMarker() const;

	void CalculateHealth();
	void CalculateAmmo();
	void Reload();
	void Dash();
	void OnFire();

	// Components attached to this class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* FirstPersonCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* SK_Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* SM_Gun = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent *Sphere = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent *CrosshairPoint = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UWidgetComponent *CrosshairWidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Monitor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* HealthMonitor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextRenderComponent* HealthCounter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* AmmoMonitor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextRenderComponent* AmmoCounter = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTimelineComponent* HealthTimeline = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UTimelineComponent* AmmoTimeline = nullptr;

	// Mesh references
	USkeletalMesh* FP_Arms = nullptr; // FP = First Person
	UStaticMesh* FP_Gun = nullptr; // FP = First Person
	UStaticMesh* UIMonitor = nullptr;
	UStaticMesh* FloorPanel = nullptr;

	// Curve references
	UCurveFloat* HealthCurveFloat = nullptr;
	UCurveFloat* AmmoCurveFloat = nullptr;

	// Material references
	UMaterialInterface* BlackMaterial = nullptr;
	UMaterialInterface* CRTBarsMaterial = nullptr;
	UMaterialInterface* DiamondUIMaterial = nullptr;
	UMaterialInterface* ShotgunUIMaterial = nullptr;
	UMaterialInterface* TextMaterial = nullptr;
	UMaterialInterface* KillBarMaterial = nullptr;
	UMaterialInstanceDynamic* DiamondUIMaterialInstance = nullptr;
	UMaterialInstanceDynamic* ShotgunUIMaterialInstance = nullptr;
	UMaterialInstanceDynamic* KillBarMaterialInstance = nullptr;

	// Particle References
	UParticleSystem* SplatterParticle = nullptr;
	UParticleSystem* GunShotSmokeParticle = nullptr;
	UParticleSystem* GunShotWaveParticle = nullptr;

	// Actor References
	AActor* HitActor = nullptr;
	AGoblinCharacter* GoblinCharacter = nullptr;
	AGruntGoblin* GruntGoblin = nullptr;
	APlayerController* PlayerController = nullptr;
	AMainWorldLevelScript* LevelScript = nullptr;

	// Component References
	UPrimitiveComponent* HitComponent = nullptr;
	FName BoneName;

private:

	// Editable variables in editor
	UPROPERTY(EditAnywhere, Category = "Player", meta = (ToolTip = "How far can the player dash in a given direction?"))
	uint16 DashDistance = 150;

	UPROPERTY(EditAnywhere, Category = "Player", meta = (ToolTip = "How long does the Dash ability take to cooldown? (in seconds)"))
	float DashCooldown = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (ToolTip = "The max health of the player"))
	uint16 MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Ammo", meta = (ToolTip = "The current ammo that the player has"))
	uint16 CurrentAmmo = MaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Ammo", meta = (ToolTip = "The max ammo that the player can have"))
	uint16 MaxAmmo = 15;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "Draw lines when firing the shotgun to visualize range"))
	bool bShowDebugLines = false;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "Display an on-screen debug message of the shotgun delay timer"))
	bool bShowShotgunDelayTimer = false;
	
	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "Display an on-screen debug message of the shotgun delay timer"))
	bool bShowDebugGoblinHit = false;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "How far should the weapon range be?"))
	float WeaponRange = 350.0f;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "How far should the weapon radius be?"))
	float SphereTraceRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "How much damage should the player output?"))
	float WeaponDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "How long does the gun take to reload? (in seconds)"))
	float ReloadTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "How fast/slow can the shotgun shoot? (<1=Fast, 1=Normal, >1=Slow)"))
	float FireRate = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Firing", meta = (ToolTip = "How intense should the kick-back be when firing? (<100=Subtle, 100=Normal, >100=Intense"))
	uint16 KickbackScale = 500;

	bool bCanShoot = true;
	bool bCanDash = true;
	bool bIsReloading = false;
	bool bIsFullAmmo = true;
	bool bReverse = false;
	bool bIsDead = false;

	float LerpSpeed = 2.0f;

	int16 NewHealth = MaxHealth;
	uint16 OldHealth = CurrentHealth;
	uint16 OldAmmo = CurrentAmmo;
	uint16 NewAmmo = MaxAmmo;
	uint16 MaxHits = 5;

	// Gun offset from the camera location
	FVector GunOffset = FVector(100.0f, 0.0f, -10.0f);
	FVector StartTrace, EndTrace;

	FAttachmentTransformRules AttachmentRuleSnap = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	FAttachmentTransformRules AttachmentRuleRelative = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

	TArray<FHitResult> BulletHitResult;
	TSubclassOf<UCameraShake> FireShake;
	TSubclassOf<UCameraShake> IdleShake;
	TSubclassOf<UCameraShake> WalkShake;
	TSubclassOf<UUserWidget> HUDClass;
	TSubclassOf<UUserWidget> CrosshairClass;

	UAnimMontage *FireMontage = nullptr;
	USoundBase *WeaponFire = nullptr;
	USoundBase *BulletHitSound = nullptr;
	USoundBase *DryFireSound = nullptr;

	// Timers
	FTimerHandle ReloadTimerHandle;
	FTimerHandle FireRateTimerHandle;
	FTimerHandle DashCooldownTimerHandle;
	FTimerHandle AmmoShellTimerHandle;
	FTimerHandle HitMarkerTimerHandle;
	FTimerHandle DestroyGoblinTimerHandle;
};

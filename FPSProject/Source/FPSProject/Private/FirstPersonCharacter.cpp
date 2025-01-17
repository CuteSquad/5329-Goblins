// Copyright © 2018, AJACKS

#include "FirstPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "ConstructorHelpers.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"
#include "UserWidget.h"
#include "MainWorldLevelScript.h"

AFirstPersonCharacter::AFirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Object initializations/loading
	static ConstructorHelpers::FObjectFinder<UClass> AnimationBlueprint(TEXT("AnimBlueprint'/Game/FirstPerson/Animations/FirstPerson_AnimBP.FirstPerson_AnimBP_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> FireShakeBP(TEXT("Blueprint'/Game/5329_Goblins/Blueprints/CameraShake/FireShake.FireShake_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> IdleShakeBP(TEXT("Blueprint'/Game/5329_Goblins/Blueprints/CameraShake/IdleShake.IdleShake_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> WalkShakeBP(TEXT("Blueprint'/Game/5329_Goblins/Blueprints/CameraShake/WalkShake.WalkShake_C'"));
	static ConstructorHelpers::FClassFinder<UHUDWidget> HUDBP(TEXT("WidgetBlueprint'/Game/5329_Goblins/UI/HUD.HUD_C'"));
	static ConstructorHelpers::FClassFinder<UHUDWidget> CrosshairBP(TEXT("WidgetBlueprint'/Game/5329_Goblins/UI/Crosshair.Crosshair_C'"));
	FP_Arms = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'")));
	FP_Gun = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/ImportedStuff/Gun_01_Final.Gun_01_Final'")));
	TextMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Engine/EngineMaterials/DefaultTextMaterialOpaque.DefaultTextMaterialOpaque'")));
	BlackMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Player/Black.Black'")));
	CRTBarsMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Player/CRTbars.CRTbars'")));
	DiamondUIMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/UI/DiamondUI_MAT_2.DiamondUI_MAT_2'")));
	ShotgunUIMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/UI/UI_AmmoShell_MAT.UI_AmmoShell_MAT'")));
	KillBarMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/UI/KillCounterBar_MAT.KillCounterBar_MAT'")));
	FireMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr, TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'")));
	WeaponFire = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundWave'/Game/5329_Goblins/Audio/ShotgunSound02.ShotgunSound02'")));
	DryFireSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundWave'/Game/5329_Goblins/Audio/DryFireSound.DryFireSound'")));
	BulletHitSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundWave'/Game/5329_Goblins/Audio/BulletHitAudio.BulletHitAudio'")));
	UIMonitor = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/5329_Goblins/Assets/Player/UI_Monitor.UI_Monitor'")));
	FloorPanel = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/5329_Goblins/Assets/Floors/Floor_Panel.Floor_Panel'")));
	SplatterParticle = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), nullptr, TEXT("ParticleSystem'/Game/ParticleWIP/Splatter_Particle.Splatter_Particle'")));
	GunShotSmokeParticle = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), nullptr, TEXT("ParticleSystem'/Game/ParticleWIP/GunShotSmoke.GunShotSmoke'")));
	GunShotWaveParticle = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), nullptr, TEXT("ParticleSystem'/Game/ImportContent/Particles/GunShotWave.GunShotWave'")));

	// Player default settings
	AutoReceiveInput = EAutoReceiveInput::Player0;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->MaxAcceleration = 5086.0f;
	GetCapsuleComponent()->SetCapsuleSize(55.0f, 168.0f);

	// Camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->SetWorldLocation(FVector(0.0f, 0.0f, 94.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Skeletal Mesh
	SK_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FP_Mesh"));
	SK_Mesh->SetupAttachment(FirstPersonCamera);
	SK_Mesh->SetRelativeLocation(FVector(-2.0f, -4.5f, -155.0f));
	SK_Mesh->SetRelativeRotation(FRotator(2.0f, -20.0f, 5.0f));
	SK_Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SK_Mesh->AnimClass = AnimationBlueprint.Object;
	SK_Mesh->SetSkeletalMesh(FP_Arms);

	// Gun Mesh
	SM_Gun = CreateDefaultSubobject<UStaticMeshComponent>(FName("FP_Gun"));
	SM_Gun->SetupAttachment(SK_Mesh);
	SM_Gun->SetStaticMesh(FP_Gun);
	SM_Gun->AttachToComponent(SK_Mesh, FAttachmentTransformRules::KeepRelativeTransform, FName("GripPoint"));
	SM_Gun->SetRelativeLocation(FVector(-0.000023f, 50.0f, -20.0f));
	SM_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Sphere (spawn point for projectile)
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->SetupAttachment(SM_Gun);
	Sphere->SetSphereRadius(4.0f);
	Sphere->SetRelativeLocation(FVector(-65.628418f, -0.285323f, 32.992214f));

	// Crosshair Point
	CrosshairPoint = CreateDefaultSubobject<USceneComponent>(FName("Crosshair Point"));
	CrosshairPoint->SetupAttachment(Sphere);

	// Crosshair Widget Component
	if (CrosshairBP.Succeeded())
		CrosshairClass = CrosshairBP.Class;

	CrosshairWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("Crosshair"));
	CrosshairWidgetComponent->SetupAttachment(CrosshairPoint);
	CrosshairWidgetComponent->SetRelativeLocation(FVector(-9.859469f, 105.663567f, 9.785245f));
	CrosshairWidgetComponent->SetRelativeScale3D(FVector(0.0f, 0.215249f, 0.0f));
	CrosshairWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	CrosshairWidgetComponent->SetWidgetClass(CrosshairClass);
	CrosshairWidgetComponent->SetDrawSize(FVector2D(1280.0f, 720.0f));
	CrosshairWidgetComponent->bEditableWhenInherited = true;

	// Monitor
	Monitor = CreateDefaultSubobject<UStaticMeshComponent>(FName("Monitor"));
	Monitor->SetupAttachment(SM_Gun);
	Monitor->SetRelativeLocation(FVector(16.300026f, 30.119410f, 5.198041f));
	Monitor->SetRelativeRotation(FRotator(-3.592346f, -176.757278f, -18.255554f));
	Monitor->SetRelativeScale3D(FVector(1.103361f, 1.209820f, 1.353995f));
	Monitor->SetMobility(EComponentMobility::Movable);
	Monitor->SetStaticMesh(UIMonitor);
	Monitor->SetMaterial(0, BlackMaterial);
	Monitor->bEditableWhenInherited = true;

	// Health Monitor
	HealthMonitor = CreateDefaultSubobject<UStaticMeshComponent>(FName("HealthMonitor"));
	HealthMonitor->SetupAttachment(Monitor);
	HealthMonitor->SetRelativeLocation(FVector(-34.955952f, 2.320272f, 14.143700f));
	HealthMonitor->SetRelativeRotation(FRotator(-19.107473f, 65.257294f, 172.731613f));
	HealthMonitor->SetRelativeScale3D(FVector(0.215642f, 0.259137f, 0.099021f));
	HealthMonitor->SetMobility(EComponentMobility::Movable);
	HealthMonitor->SetStaticMesh(FloorPanel);
	HealthMonitor->SetMaterial(0, DiamondUIMaterial);
	HealthMonitor->bEditableWhenInherited = true;

	// Health Counter
	HealthCounter = CreateDefaultSubobject<UTextRenderComponent>(FName("HealthCounter"));
	HealthCounter->SetupAttachment(SM_Gun);
	HealthCounter->SetRelativeLocation(FVector(31.435530f ,17.615507f, 33.073792f));
	HealthCounter->SetRelativeRotation(FRotator(-14.698048f, 149.420425f, 6.159955f));
	HealthCounter->SetRelativeScale3D(FVector(0.372016f, -0.077510f, 0.083184f));
	HealthCounter->SetText(FText::AsNumber(MaxHealth));
	HealthCounter->SetWorldSize(48.0f);
	HealthCounter->SetMaterial(0, TextMaterial);
	HealthCounter->SetHorizontalAlignment(EHTA_Center);
	HealthCounter->SetVerticalAlignment(EVRTA_TextCenter);

	// Ammo Monitor
	AmmoMonitor = CreateDefaultSubobject<UStaticMeshComponent>(FName("AmmoMonitor"));
	AmmoMonitor->SetupAttachment(Monitor);
	AmmoMonitor->SetRelativeLocation(FVector(-48.978195f, 34.207783f, 16.128361f));
	AmmoMonitor->SetRelativeRotation(FRotator(-17.423832f, 89.410461f, 119.947197f));
	AmmoMonitor->SetRelativeScale3D(FVector(0.164702f, 2.839705f, 0.071209f));
	AmmoMonitor->SetMobility(EComponentMobility::Movable);
	AmmoMonitor->SetStaticMesh(FloorPanel);
	AmmoMonitor->SetMaterial(0, ShotgunUIMaterial);
	AmmoMonitor->bEditableWhenInherited = true;

	// Ammo Counter
	AmmoCounter = CreateDefaultSubobject<UTextRenderComponent>(FName("AmmoCounter"));
	AmmoCounter->SetupAttachment(SM_Gun);
	AmmoCounter->SetRelativeLocation(FVector(43.043049f, 0.262630f, 39.417923f));
	AmmoCounter->SetRelativeRotation(FRotator(-39.415924f, -179.077789f, -0.049913f));
	AmmoCounter->SetRelativeScale3D(FVector(-0.131584f, -0.105816f, 0.116409f));
	AmmoCounter->SetText(FText::AsNumber(MaxAmmo));
	AmmoCounter->SetWorldSize(28.0f);
	AmmoCounter->SetHorizSpacingAdjust(-2.0f);
	AmmoCounter->SetMaterial(0, TextMaterial);
	AmmoCounter->SetHorizontalAlignment(EHTA_Center);
	AmmoCounter->SetVerticalAlignment(EVRTA_TextCenter);

	// Health timeline Component
	HealthTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Health Timeline"));
	HealthCurveFloat = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/5329_Goblins/CurveFloats/HealthLerpTime.HealthLerpTime'")));

	// Ammo TimelineComponent
	AmmoTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Ammo Timeline"));
	AmmoCurveFloat = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/5329_Goblins/CurveFloats/AmmoLerpTime.AmmoLerpTime'")));

	// Nullptr checks
	if (FireShakeBP.Object)
		FireShake = FireShakeBP.Object;

	if (IdleShakeBP.Object)
		IdleShake = IdleShakeBP.Object;

	if (WalkShakeBP.Object)
		WalkShake = WalkShakeBP.Object;

	if (HUDBP.Succeeded())
		HUDClass = HUDBP.Class;
}

void AFirstPersonCharacter::AddHealth(int Amount)
{
	OldHealth = CurrentHealth;
	NewHealth += Amount;
	NewHealth = FMath::Clamp(int(NewHealth), 0, int(MaxHealth));
	CalculateHealth();
}

void AFirstPersonCharacter::ApplyDamage(int GoblinAttackDamage)
{
	OldHealth = CurrentHealth;
	NewHealth -= GoblinAttackDamage;

	if (NewHealth > 0)
		NewHealth = FMath::Clamp(int(NewHealth), 0, int(MaxHealth));

	UGameplayStatics::SpawnEmitterAttached(SplatterParticle, FirstPersonCamera);
	HUD->PlayAnimation(HUD->DamageAnimation, 0);
	CalculateHealth();
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Attachments
	FirstPersonCamera->AttachToComponent(RootComponent, AttachmentRuleRelative);
	SK_Mesh->AttachToComponent(FirstPersonCamera, AttachmentRuleRelative);
	SM_Gun->AttachToComponent(SK_Mesh, AttachmentRuleRelative, FName("GripPoint"));
	Monitor->AttachToComponent(SM_Gun, AttachmentRuleRelative);
	HealthMonitor->AttachToComponent(Monitor, AttachmentRuleRelative);
	AmmoMonitor->AttachToComponent(Monitor, AttachmentRuleRelative);
	AmmoCounter->AttachToComponent(SM_Gun, AttachmentRuleRelative);
	HealthCounter->AttachToComponent(SM_Gun, AttachmentRuleRelative);
	Sphere->AttachToComponent(SM_Gun, AttachmentRuleRelative);
	CrosshairPoint->AttachToComponent(Sphere, AttachmentRuleRelative);
	CrosshairWidgetComponent->AttachToComponent(CrosshairPoint, AttachmentRuleRelative);

	// Variable Initializations
	LerpSpeed = 1.5f;
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	LevelScript = Cast<AMainWorldLevelScript>(GetWorld()->GetLevelScriptActor());

	// Player setup
	CurrentHealth = MaxHealth;
	HealthCounter->SetText(FText::AsNumber(MaxHealth));
	CurrentAmmo = MaxAmmo;
	AmmoCounter->SetText(FText::AsNumber(MaxAmmo));

	// Material Instance setup
	DiamondUIMaterialInstance = UMaterialInstanceDynamic::Create(DiamondUIMaterial, this);
	ShotgunUIMaterialInstance = UMaterialInstanceDynamic::Create(ShotgunUIMaterial, this);
	KillBarMaterialInstance = UMaterialInstanceDynamic::Create(KillBarMaterial, this);
	HealthMonitor->SetMaterial(0, DiamondUIMaterialInstance);
	AmmoMonitor->SetMaterial(0, ShotgunUIMaterialInstance);

	// HealthTimeline Initialization
	FOnTimelineFloat OnHealthTimelineCallback;
	OnHealthTimelineCallback.BindUFunction(this, "OnCalculateHealth");

	if (HealthCurveFloat != nullptr)
	{
		HealthTimeline = NewObject<UTimelineComponent>(this, FName("HealthLerpSpeed"));
		HealthTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		HealthTimeline->SetPropertySetObject(this);
		HealthTimeline->SetLooping(false);
		HealthTimeline->SetPlaybackPosition(0.0f, false);
		HealthTimeline->SetPlayRate(LerpSpeed);
		HealthTimeline->AddInterpFloat(HealthCurveFloat, OnHealthTimelineCallback);
		HealthTimeline->SetTimelineLength(1.0f / LerpSpeed);
		HealthTimeline->SetTimelineLengthMode(TL_TimelineLength);
		HealthTimeline->RegisterComponent();

		HealthCurveFloat->FloatCurve.Reset();
		HealthCurveFloat->FloatCurve.UpdateOrAddKey(0.0, 0.0f); // Time: 0  Value: 0
		HealthCurveFloat->FloatCurve.UpdateOrAddKey(1.0f / LerpSpeed, 1.0f); // Time: LerpSpeed  Value: 1
	}

	// AmmoTimeline Initialization
	FOnTimelineFloat OnAmmoTimelineCallback;
	OnAmmoTimelineCallback.BindUFunction(this, "OnCalculateAmmo");

	if (AmmoCurveFloat != nullptr)
	{
		AmmoTimeline = NewObject<UTimelineComponent>(this, FName("AmmoShellLerpSpeed"));
		AmmoTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		AmmoTimeline->SetPropertySetObject(this);
		AmmoTimeline->SetLooping(false);
		AmmoTimeline->SetPlaybackPosition(0.0f, false);
		AmmoTimeline->SetPlayRate(LerpSpeed);
		AmmoTimeline->AddInterpFloat(AmmoCurveFloat, OnAmmoTimelineCallback);
		AmmoTimeline->SetTimelineLength(1.0f / LerpSpeed);
		AmmoTimeline->SetTimelineLengthMode(TL_TimelineLength);
		AmmoTimeline->RegisterComponent();

		AmmoCurveFloat->FloatCurve.Reset();
		AmmoCurveFloat->FloatCurve.UpdateOrAddKey(0.0, 0.0f); // Time: 0  Value: 0
		AmmoCurveFloat->FloatCurve.UpdateOrAddKey(1.0f / LerpSpeed, 1.0f); // Time: LerpSpeed  Value: 1
	}

	// Display the HUD Widget
	HUD = CreateWidget<UHUDWidget>(PlayerController, HUDClass);
	HUD->AddToViewport(9999); // Render on the very top
	HUD->KillBar->SetBrushFromMaterial(KillBarMaterialInstance);

	Crosshair = Cast<UHUDWidget>(CrosshairWidgetComponent->GetUserWidgetObject());
}

void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Hook the timeline tick component with our tick so that they match
	if (HealthTimeline != nullptr)
		HealthTimeline->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);

	if (AmmoTimeline != nullptr)
		AmmoTimeline->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);

	if (bShowShotgunDelayTimer)
	{
		if (GetWorldTimerManager().IsTimerActive(FireRateTimerHandle))
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("Delay: ") + FString::SanitizeFloat(GetWorldTimerManager().GetTimerRemaining(FireRateTimerHandle)), true);
	}

	if (GetVelocity().Size() > 0 && CanJump())
	{
		// Shake camera (Walking shake)
		PlayerController->ClientPlayCameraShake(WalkShake, 1.0f);
	}
	else
	{
		// Shake camera (breathing shake)
		PlayerController->ClientPlayCameraShake(IdleShake, 1.0f);
	}
}

void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis bindings
	InputComponent->BindAxis(FName("MoveForward"), this, &AFirstPersonCharacter::MoveXAxis);
	InputComponent->BindAxis(FName("MoveRight"), this, &AFirstPersonCharacter::MoveYAxis);
	InputComponent->BindAxis(FName("Turn"), this, &AFirstPersonCharacter::AddControllerYawInput);
	InputComponent->BindAxis(FName("LookUp"), this, &AFirstPersonCharacter::AddControllerPitchInput);

	// Action bindings
	InputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AFirstPersonCharacter::Dash);
	InputComponent->BindAction(FName("Fire"), IE_Pressed, this, &AFirstPersonCharacter::Fire);
	InputComponent->BindAction(FName("Reload"), IE_Pressed, this, &AFirstPersonCharacter::Reload);
}

void AFirstPersonCharacter::Fire()
{
	if (bCanShoot && !bIsReloading && CurrentAmmo > 0)
	{
		if (!GetWorldTimerManager().IsTimerActive(FireRateTimerHandle))
		{
			OnFire();
			bCanShoot = false;
			GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AFirstPersonCharacter::SetFireState, FireRate, false);
		}
	}
	else if (CurrentAmmo == 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DryFireSound, GetActorLocation());
	}
}

void AFirstPersonCharacter::OnCalculateHealth()
{
	// Get the value of the timeline and feed into the curve float for lerping between health values
	const float TimelineValue = HealthTimeline->GetPlaybackPosition();
	const float CurveFloatValue = HealthCurveFloat->GetFloatValue(TimelineValue);

	// Lerp between the two health values and clamp
	NewHealth = FMath::Clamp(int(NewHealth), 0, int(MaxHealth));
	CurrentHealth = FMath::Lerp(float(OldHealth), float(NewHealth), CurveFloatValue);
	FMath::Clamp(int(CurrentHealth), 0, int(MaxHealth));
	HealthCounter->SetText(FText::AsNumber(CurrentHealth));

	if (CurrentHealth != 0)
	{
		// Update health bar to reflect the current health
		DiamondUIMaterialInstance->SetScalarParameterValue(FName("ScalarHealth"), float(CurrentHealth) / float(MaxHealth));
	}

	// Health checks
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0, int(MaxHealth));
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		LevelScript->OnPlayerDeath();
		HUD->PlayAnimation(HUD->GoblinsKilledTextAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
		HUD->PlayAnimation(HUD->GoblinsKilledAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
		Crosshair->PlayAnimation(Crosshair->CrosshairAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
		HUD->DeadTextBlock->SetVisibility(ESlateVisibility::Visible);
		HUD->PlayAnimation(HUD->DeadTextAnim, 0.0f);
		return;
	}

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
		FMath::Clamp(int(CurrentHealth) / int(MaxHealth), 0, int(MaxHealth));
		HealthCounter->SetText(FText::AsNumber(CurrentHealth));
	}
}

void AFirstPersonCharacter::OnCalculateAmmo()
{
	// Get the value of the timeline and feed into the curve float for lerping between health values
	const float TimelineValue = AmmoTimeline->GetPlaybackPosition();
	const float CurveFloatValue = AmmoCurveFloat->GetFloatValue(TimelineValue);

	if (bReverse)
	{
		CurrentAmmo = FMath::Lerp(float(OldAmmo), float(MaxAmmo), CurveFloatValue);
		FMath::Clamp(int(CurrentAmmo), 0, int(MaxAmmo));
		AmmoCounter->SetText(FText::AsNumber(CurrentAmmo));
	}
	else
	{
		CurrentAmmo = FMath::Lerp(float(OldAmmo), float(NewAmmo), CurveFloatValue);
		FMath::Clamp(int(CurrentAmmo), 0, int(MaxAmmo));
		AmmoCounter->SetText(FText::AsNumber(CurrentAmmo));
	}

	if (CurrentAmmo <= 0)
	{
		// Disable shooting and update the ammo monitor
		bCanShoot = false;
		AmmoCounter->SetText(FText::AsCultureInvariant("R"));
		ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor"), FLinearColor(1.0f, 0.0f, 0.025124)); // Update ammo shell color to Red
		ShotgunUIMaterialInstance->SetScalarParameterValue(FName("ScalarAmmo"), 1.0f); // Update ammo shell
		ShotgunUIMaterialInstance->SetScalarParameterValue(FName("Frequency"), 0.75f); // Update ammo shell pulsating frequency
		ShotgunUIMaterialInstance->SetScalarParameterValue(FName("MinBrightness"), 0.0f); // Update ammo shell minimum brightness
	}
	else
		ShotgunUIMaterialInstance->SetScalarParameterValue(FName("ScalarAmmo"), float(CurrentAmmo) / float(MaxAmmo)); // Update ammo shell when we have ammo
}

void AFirstPersonCharacter::OnReload()
{
	// Check if we are reloading then reset ammo
	if (bIsReloading)
	{
		CurrentAmmo = MaxAmmo;
		NewAmmo = MaxAmmo;
		AmmoCounter->SetText(FText::AsNumber(MaxAmmo));

		// Is reloading done?
		if (GetWorldTimerManager().GetTimerRemaining(ReloadTimerHandle) == 0)
		{
			bCanShoot = true;
			bIsReloading = false;
			bReverse = false;

			ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor"), FLinearColor(0.714583f, 0.0f, 1.0f)); // Update ammo shell color to Pink
			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("ScalarAmmo"), float(CurrentAmmo) / float(MaxAmmo)); // Update ammo shell
			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("Frequency"), 0.25f); // Update ammo shell pulsating frequency
			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("MinBrightness"), 5.0f); // Update ammo shell minimum brightness
		}
	}
}

void AFirstPersonCharacter::SetDashState()
{
	if (bCanDash)
		bCanDash = false;
	else
		bCanDash = true;
}

void AFirstPersonCharacter::OnFire()
{
	//Fire weapon if we can shoot
	if (bCanShoot)
	{
		// Play fire animation
		SK_Mesh->GetAnimInstance()->Montage_Play(FireMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f);

		// Play sound
		UGameplayStatics::PlaySoundAtLocation(this, WeaponFire, GetActorLocation());

		// Spawn Particles
		UGameplayStatics::SpawnEmitterAttached(GunShotSmokeParticle, Sphere, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, FVector(0.1f, 0.1f, 0.1f));
		UGameplayStatics::SpawnEmitterAttached(GunShotWaveParticle, Sphere, NAME_None, FVector(ForceInit), FRotator(0.0f, 0.0f, 90.0f), FVector(6.0f, 6.0f, 6.0f));

		// Calculate Sphere trace
		const float FPCameraPitch = FirstPersonCamera->GetComponentRotation().Pitch;

		StartTrace = Sphere->GetComponentLocation();
		EndTrace = StartTrace + (-Sphere->GetForwardVector() * WeaponRange);

		const FCollisionShape CapsuleCollision = FCollisionShape::MakeCapsule(SphereTraceRadius, WeaponRange);
		FCollisionObjectQueryParams CollisionParams;
		CollisionParams.AddObjectTypesToQuery(ECC_GameTraceChannel2); // Goblin

		// DEBUG
		if (bShowDebugLines)
			DrawDebugCapsule(GetWorld(), EndTrace, WeaponRange, SphereTraceRadius, FQuat::MakeFromEuler(FVector(GetActorRotation().Roll, FPCameraPitch - 90, GetActorRotation().Yaw)), FColor::White, true, -1, 0, 1);

		// Check which actor was hit by the sphere trace and destroy it
		if (GetWorld()->SweepMultiByObjectType(BulletHitResult, EndTrace, EndTrace, FQuat::MakeFromEuler(FVector(GetActorRotation().Roll, FPCameraPitch - 90, GetActorRotation().Yaw)), CollisionParams, CapsuleCollision))
		{
			TArray<AActor*> HitActors;
			TArray<UPrimitiveComponent*> HitComponents;
			TArray<FName> BoneNames;

			// Find duplicates in the BulletHitResult array
			for (auto HitResult : BulletHitResult)
			{
				if (HitActors.Num() < MaxHits)
				{
					HitActors.AddUnique(HitResult.GetActor()); // Add only the unique items to the HitActors array (no duplicates)
					HitComponents.AddUnique(HitResult.GetComponent()); // Add only the unique items to the HitComponents array (no duplicates)
					BoneNames.AddUnique(HitResult.BoneName); // Add only the unique items to the BoneNames array (no duplicates)
				}
				else
					break;
			}

			for (uint16 i = 0; i < HitActors.Num(); i++)
			{
				HitActor = HitActors[i];
				HitComponent = HitComponents[i];
				BoneName = BoneNames[i];

				if (IsValid(HitActor) && this != HitActor)
				{
					if (HitActor->IsA(AGoblinCharacter::StaticClass()))
					{
						// Convert to GoblinCharacter
						GoblinCharacter = Cast<AGoblinCharacter>(HitActor);

						if (GoblinCharacter->CurrentHealth > 0)
						{
							// Show Hitmarker and play Bullet hit sound
							HUD->HitMarkerImage->SetVisibility(ESlateVisibility::Visible);
							HUD->PlayAnimation(HUD->HitMarkerAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 2.0f);
							GetWorldTimerManager().SetTimer(HitMarkerTimerHandle, this, &AFirstPersonCharacter::RemoveHitMarker, 0.2f);
							UGameplayStatics::PlaySoundAtLocation(this, BulletHitSound, GoblinCharacter->GetActorLocation());

							if (bShowDebugGoblinHit)
								GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, HitActor->GetName() + " is at " + FString::FromInt(GoblinCharacter->CurrentHealth), true);
						}

						GoblinCharacter->CurrentHealth -= WeaponDamage;
						GoblinCharacter->bIsGoblinHit = true;

						if (GoblinCharacter->CurrentHealth <= 0)
						{
							// Enable Ragdoll physics
							HitComponent->SetSimulatePhysics(true);
							HitComponent->SetMobility(EComponentMobility::Movable);
							HitComponent->AddImpulseAtLocation(this->GetActorRotation().Vector() * WeaponDamage, HitComponent->GetComponentLocation(), BoneName);

							// We do not want to collide with the goblin after it is dead
							GoblinCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
							GoblinCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
							GoblinCharacter->GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
							GoblinCharacter->GoblinAnimInstance->bHealthBelowZero = true;

							// Display the new kill count
							GoblinsKilled += 1;
							GoblinsKilledForDoorUnlock += 1;
							HUD->NumOfGoblinsText = FString::FromInt(GoblinsKilled) + FString(" Goblins Killed");

							GoblinCharacter->Dissolve();

							GoblinCharacter->SetLifeSpan(1.5f);
						}
					}
					else if (HitActor->IsA(AGruntGoblin::StaticClass()))
					{
						// Convert to GruntGoblin
						GruntGoblin = Cast<AGruntGoblin>(HitActor);

						if (GruntGoblin->CurrentHealth > 0)
						{
							// Show Hitmarker and play Bullet hit sound
							HUD->HitMarkerImage->SetVisibility(ESlateVisibility::Visible);
							HUD->PlayAnimation(HUD->HitMarkerAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 2.0f);
							GetWorldTimerManager().SetTimer(HitMarkerTimerHandle, this, &AFirstPersonCharacter::RemoveHitMarker, 0.2f);
							UGameplayStatics::PlaySoundAtLocation(this, BulletHitSound, GruntGoblin->GetActorLocation());

							if (bShowDebugGoblinHit)
								GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, HitActor->GetName() + " is at " + FString::FromInt(GruntGoblin->CurrentHealth), true);
						}

						GruntGoblin->CurrentHealth -= WeaponDamage;
						GoblinCharacter->bIsGoblinHit = true;

						if (GruntGoblin->CurrentHealth <= 0)
						{
							// Enable Ragdoll physics
							HitComponent->SetSimulatePhysics(true);
							HitComponent->SetMobility(EComponentMobility::Movable);
							HitComponent->AddImpulseAtLocation(this->GetActorRotation().Vector() * WeaponDamage, HitComponent->GetComponentLocation(), BoneName);

							// We do not want to collide with the goblin after it is dead
							GoblinCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
							GoblinCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
							GoblinCharacter->GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
							GruntGoblin->GoblinAnimInstance->bHealthBelowZero = true;

							// Display the new kill count
							GoblinsKilled += 1;
							GoblinsKilledForDoorUnlock += 1;
							HUD->NumOfGoblinsText = FString::FromInt(GoblinsKilled) + FString(" Goblins Killed");

							GoblinCharacter->Dissolve();

							GoblinCharacter->SetLifeSpan(1.5f);
						}
					}
				}
			}
		}

		// Kickback when firing
		const FVector Velocity = FVector(-GetActorForwardVector() * KickbackScale);
		LaunchCharacter(Velocity, true, true);

		// Shake the camera
		PlayerController->ClientPlayCameraShake(FireShake, 1.0f);

		// Update the kill bar
		KillBarMaterialInstance->SetScalarParameterValue(FName("ScalarKills"), float(GoblinsKilled) / 5329.0f);

		// Calculate Ammo
		OldAmmo = CurrentAmmo;
		CalculateAmmo();
	}
}

void AFirstPersonCharacter::SetFireState()
{
	if (bCanShoot)
		bCanShoot = false;
	else
		bCanShoot = true;
}

void AFirstPersonCharacter::RemoveHitMarker() const
{
	HUD->PlayAnimation(HUD->HitMarkerAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 2.0f);
}

void AFirstPersonCharacter::CalculateHealth()
{
	if (NewHealth <= 0)
	{
		CurrentHealth = FMath::Clamp(NewHealth, int16(0), int16(0));
		if (CurrentHealth <= 0)
		{
			CurrentHealth = 0;
			CurrentHealth = FMath::Clamp(CurrentHealth, 0, int(MaxHealth));
			DisableInput(UGameplayStatics::GetPlayerController(this, 0));
			LevelScript->OnPlayerDeath();
			HUD->PlayAnimation(HUD->GoblinsKilledTextAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
			HUD->PlayAnimation(HUD->GoblinsKilledAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
			HUD->PlayAnimation(HUD->KillBarAnim, 0.0, 1, EUMGSequencePlayMode::Reverse, 1.0f);
			HUD->PlayAnimation(HUD->KillBarOutlineAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
			HUD->PlayAnimation(HUD->TotalGoblinsAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
			Crosshair->PlayAnimation(Crosshair->CrosshairAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 2.0f);
			HUD->DeadTextBlock->SetVisibility(ESlateVisibility::Visible);
			HUD->PlayAnimation(HUD->DeadTextAnim, 0.0f);
		}
	}

	HealthTimeline->PlayFromStart(); // Will start execute OnCalculateHealth every tick of the timeline duration (this is used for lerping the diamond health bar)

}

void AFirstPersonCharacter::CalculateAmmo()
{
	NewAmmo -= 1;
	AmmoTimeline->PlayFromStart();
}

void AFirstPersonCharacter::Reload()
{
	// Ammo checks
	if (CurrentAmmo == MaxAmmo)
		bIsFullAmmo = true;
	else
		bIsFullAmmo = false;

	// Only reload when we don't have full ammo and not firing
	if (!bIsReloading && !bIsFullAmmo)
	{
		// Wait X seconds before executing the OnReload function (X = ReloadTime)
		bCanShoot = false;
		bIsReloading = true;
		bReverse = true;
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AFirstPersonCharacter::OnReload, ReloadTime, false);
		AmmoTimeline->SetPlayRate(ReloadTime * 0.3f);
		AmmoTimeline->PlayFromStart();
	}
}

void AFirstPersonCharacter::Dash()
{
	if (bCanDash)
	{
		if (!GetWorldTimerManager().IsTimerActive(FireRateTimerHandle))
		{
			// Do the dash
			const FVector Velocity = FVector(((GetInputAxisValue(FName("MoveForward")) * DashDistance) * GetActorForwardVector()) + (GetInputAxisValue(FName("MoveRight")) * DashDistance) * GetActorRightVector());
			LaunchCharacter(Velocity * DashDistance / 8, true, true);

			// Start the cooldown timer
			bCanDash = false;
			GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, this, &AFirstPersonCharacter::SetDashState, DashCooldown, false);
		}
	}
}

void AFirstPersonCharacter::MoveXAxis(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), GetInputAxisValue(FName("MoveForward")));
}

void AFirstPersonCharacter::MoveYAxis(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), GetInputAxisValue(FName("MoveRight")));
}

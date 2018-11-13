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

AFirstPersonCharacter::AFirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Object initializations/loading
	static ConstructorHelpers::FObjectFinder<UClass> AnimationBlueprint(TEXT("AnimBlueprint'/Game/FirstPerson/Animations/FirstPerson_AnimBP.FirstPerson_AnimBP_C'"));
	FP_Arms = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'")));
	FP_Gun = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun.SK_FPGun'")));
	TextMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Engine/EngineMaterials/DefaultTextMaterialOpaque.DefaultTextMaterialOpaque'")));
	BlackMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Player/Black.Black'")));
	CRTBarsMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Player/CRTbars.CRTbars'")));
	DiamondUIMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/UI/DiamondUI_MAT_2.DiamondUI_MAT_2'")));
	ShotgunUIMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/UI/UI_AmmoShell_MAT.UI_AmmoShell_MAT'")));
	FireMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr, TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'")));
	WeaponFire = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundWave'/Game/FirstPerson/Audio/FirstPersonTemplateWeaponFire02.FirstPersonTemplateWeaponFire02'")));
	UIMonitor = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/5329_Goblins/Assets/Player/UI_Monitor.UI_Monitor'")));
	FloorPanel = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/5329_Goblins/Assets/Floors/Floor_Panel.Floor_Panel'")));

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

	// Gun Skeletal Mesh
	SK_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FP_Gun"));
	SK_Gun->SetupAttachment(SK_Mesh);
	SK_Gun->SetSkeletalMesh(FP_Gun);
	SK_Gun->AttachToComponent(SK_Mesh, FAttachmentTransformRules::KeepRelativeTransform, FName("GripPoint"));

	// Sphere (spawn point for projectile)
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->SetupAttachment(SK_Gun);
	Sphere->SetSphereRadius(4.0f);
	Sphere->SetRelativeLocation(FVector(0.0f, 56.0f, 11.0f));

	// Monitor
	Monitor = CreateDefaultSubobject<UStaticMeshComponent>(FName("Monitor"));
	Monitor->SetupAttachment(SK_Gun);
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
	HealthMonitor->SetRelativeLocation(FVector(-8.513375f, 4.828093f, -0.414496f));
	HealthMonitor->SetRelativeRotation(FRotator(-0.000549f, -0.000946f, -179.999939f));
	HealthMonitor->SetRelativeScale3D(FVector(0.090038f, 0.001000f, 0.074806f));
	HealthMonitor->SetMobility(EComponentMobility::Movable);
	HealthMonitor->SetStaticMesh(FloorPanel);
	HealthMonitor->SetMaterial(0, DiamondUIMaterial);
	HealthMonitor->bEditableWhenInherited = true;

	// Health Counter
	HealthCounter = CreateDefaultSubobject<UTextRenderComponent>(FName("HealthCounter"));
	HealthCounter->SetupAttachment(SK_Gun);
	HealthCounter->SetRelativeLocation(FVector(20.371742, 25.924501, 11.786989));
	HealthCounter->SetRelativeRotation(FRotator(-20.032705, 90.124283, 4.537992));
	HealthCounter->SetRelativeScale3D(FVector(0.372016, -0.057892, 0.072993));
	HealthCounter->SetText(FText::AsNumber(MaxHealth));
	HealthCounter->SetWorldSize(48.0f);
	HealthCounter->SetMaterial(0, TextMaterial);
	HealthCounter->SetHorizontalAlignment(EHTA_Center);
	HealthCounter->SetVerticalAlignment(EVRTA_TextCenter);

	// Ammo Monitor
	AmmoMonitor = CreateDefaultSubobject<UStaticMeshComponent>(FName("AmmoMonitor"));
	AmmoMonitor->SetupAttachment(Monitor);
	AmmoMonitor->SetRelativeLocation(FVector(-2.463829f, 4.856749f, -0.242476f));
	AmmoMonitor->SetRelativeRotation(FRotator(0.299647f, -0.107086f, -178.468033f));
	AmmoMonitor->SetRelativeScale3D(FVector(0.122178f, 0.001676f, 0.071209f));
	AmmoMonitor->SetMobility(EComponentMobility::Movable);
	AmmoMonitor->SetStaticMesh(FloorPanel);
	AmmoMonitor->SetMaterial(0, ShotgunUIMaterial);
	AmmoMonitor->bEditableWhenInherited = true;

	// Ammo Counter
	AmmoCounter = CreateDefaultSubobject<UTextRenderComponent>(FName("AmmoCounter"));
	AmmoCounter->SetupAttachment(SK_Gun);
	AmmoCounter->SetRelativeLocation(FVector(12.25, 26.233547, 13.665541));
	AmmoCounter->SetRelativeRotation(FRotator(-19.999941, 89.999878, 3.497171));
	AmmoCounter->SetRelativeScale3D(FVector(-0.131584, -0.141602, 0.116409));
	AmmoCounter->SetText(FText::AsNumber(MaxAmmo));
	AmmoCounter->SetWorldSize(28.0f);
	AmmoCounter->SetHorizSpacingAdjust(-2.0f);
	AmmoCounter->SetMaterial(0, TextMaterial);
	AmmoCounter->SetHorizontalAlignment(EHTA_Center);
	AmmoCounter->SetVerticalAlignment(EVRTA_TextCenter);

	// CRT Bars
	CRTBars = CreateDefaultSubobject<UStaticMeshComponent>(FName("CRTBars"));
	CRTBars->SetupAttachment(Monitor);
	CRTBars->SetRelativeLocation(FVector(-8.545621, 4.904549, -1.227119));
	CRTBars->SetRelativeRotation(FRotator(0.368359, -0.140930, 179.738388));
	CRTBars->SetRelativeScale3D(FVector(0.172888, 0.000827, 0.084859));
	CRTBars->SetMobility(EComponentMobility::Movable);
	CRTBars->SetStaticMesh(FloorPanel);
	CRTBars->SetMaterial(0, CRTBarsMaterial);
	CRTBars->bEditableWhenInherited = true;

	// Health timeline Component
	HealthTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Health Timeline"));
	HealthCurveFloat = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/5329_Goblins/CurveFloats/HealthLerpTime.HealthLerpTime'")));

	// Ammo TimelineComponent
	AmmoTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Ammo Timeline"));
	AmmoCurveFloat = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/5329_Goblins/CurveFloats/AmmoLerpTime.AmmoLerpTime'")));
}

void AFirstPersonCharacter::AddHealth(int Amount)
{
	OldHealth = CurrentHealth;
	NewHealth += Amount;
	NewHealth = FMath::Clamp(int(NewHealth), 0, int(MaxHealth));
	CalculateHealth();
}

void AFirstPersonCharacter::MinusHealth()
{
	OldHealth = CurrentHealth;
	NewHealth -= Damage;
	NewHealth = FMath::Clamp(int(NewHealth), 0, int(MaxHealth));
	CalculateHealth();
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Attachments
	FirstPersonCamera->AttachToComponent(RootComponent, AttachmentRuleRelative);
	SK_Mesh->AttachToComponent(FirstPersonCamera, AttachmentRuleRelative);
	SK_Gun->AttachToComponent(SK_Mesh, AttachmentRuleRelative, FName("GripPoint"));
	Monitor->AttachToComponent(SK_Gun, AttachmentRuleRelative);
	HealthMonitor->AttachToComponent(Monitor, AttachmentRuleRelative);
	AmmoMonitor->AttachToComponent(Monitor, AttachmentRuleRelative);
	CRTBars->AttachToComponent(Monitor, AttachmentRuleRelative);
	AmmoCounter->AttachToComponent(SK_Gun, AttachmentRuleRelative);
	HealthCounter->AttachToComponent(SK_Gun, AttachmentRuleRelative);
	Sphere->AttachToComponent(SK_Gun, AttachmentRuleRelative);

	// Variable Initializations
	LerpSpeed = 1.5f;

	// Player setup
	CurrentHealth = MaxHealth;
	HealthCounter->SetText(FText::AsNumber(MaxHealth));
	CurrentAmmo = MaxAmmo;
	AmmoCounter->SetText(FText::AsNumber(MaxAmmo));

	// Material Instance setup
	DiamondUIMaterialInstance = UMaterialInstanceDynamic::Create(DiamondUIMaterial, this);
	ShotgunUIMaterialInstance = UMaterialInstanceDynamic::Create(ShotgunUIMaterial, this);
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
}

void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Hook the timeline tick component with our tick so that they match
	if (HealthTimeline != nullptr)
		HealthTimeline->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);

	if (bShowShotgunDelayTimer)
	{
		if (GetWorldTimerManager().IsTimerActive(FireRateTimerHandle))
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("Delay: ") + FString::SanitizeFloat(GetWorldTimerManager().GetTimerRemaining(FireRateTimerHandle)), true);
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
	InputComponent->BindAction(FName("DEBUG_TakeHealth"), IE_Pressed, this, &AFirstPersonCharacter::MinusHealth);
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
}

void AFirstPersonCharacter::OnCalculateHealth()
{
	// Get the value of the timeline and feed into the curve float for lerping between health values
	const float TimelineValue = HealthTimeline->GetPlaybackPosition();
	const float CurveFloatValue = HealthCurveFloat->GetFloatValue(TimelineValue);

	// Lerp between the two health values and clamp
	CurrentHealth = FMath::Lerp(float(OldHealth), float(NewHealth), CurveFloatValue);
	FMath::Clamp(int(CurrentHealth), 0, int(MaxHealth));
	HealthCounter->SetText(FText::AsNumber(CurrentHealth));

	// Update health bar to reflect the current health
	DiamondUIMaterialInstance->SetScalarParameterValue(FName("ScalarHealth"), float(CurrentHealth) / float(MaxHealth));

	// Health checks
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		FMath::Clamp(CurrentHealth, 0, int(MaxHealth));
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
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

	CurrentAmmo = FMath::Lerp(float(OldAmmo), float(NewAmmo), CurveFloatValue);;
	FMath::Clamp(int(CurrentAmmo), 0, int(MaxAmmo));
	AmmoCounter->SetText(FText::AsNumber(CurrentAmmo));

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

			ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor"), FLinearColor(0.714583f, 0.0f, 1.0f)); // Update ammo shell color to Pink
			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("ScalarAmmo"), float(CurrentAmmo) / float(MaxAmmo)); // Update ammo shell
			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("Frequency"), 0.25f); // Update ammo shell pulsating frequency
			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("MinBrightness"), 5.0f); // Update ammo shell minimum brightness
		}
	}
}

void AFirstPersonCharacter::OnFire()
{
	if (bCanShoot)
	{
		///Fire weapon
		// Play fire animation
		SK_Mesh->GetAnimInstance()->Montage_Play(FireMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f);

		// Spawn projectile and play sound
		UGameplayStatics::PlaySoundAtLocation(this, WeaponFire, GetActorLocation());

		// Calculate Sphere trace
		const float FPCameraPitch = FirstPersonCamera->GetComponentRotation().Pitch;
		const FVector SphereRight = Sphere->GetRightVector();

		StartTrace = Sphere->GetComponentLocation();
		EndTrace = StartTrace + (Sphere->GetRightVector() * WeaponRange);

		const FCollisionShape CapsuleCollision = FCollisionShape::MakeCapsule(SphereTraceRadius, WeaponRange);
		FCollisionObjectQueryParams CollisionParams;
		CollisionParams.AddObjectTypesToQuery(ECC_GameTraceChannel2); // Goblin

		// DEBUG
		if (bShowDebugLines)
			DrawDebugCapsule(GetWorld(), StartTrace + (SphereRight * WeaponRange), WeaponRange, SphereTraceRadius, FQuat::MakeFromEuler(FVector(GetActorRotation().Roll, FPCameraPitch - 90, GetActorRotation().Yaw)), FColor::White, true, -1, 0, 1);

		// Check which actor was hit by the sphere trace and destroy it
		if (GetWorld()->SweepMultiByObjectType(BulletHitResult, StartTrace + (SphereRight * WeaponRange), EndTrace, FQuat::MakeFromEuler(FVector(GetActorRotation().Roll, FPCameraPitch - 90, GetActorRotation().Yaw)), CollisionParams, CapsuleCollision))
		{
			TArray<AActor*> HitActors;
			TArray<UPrimitiveComponent*> HitComponents;
			TArray<FName> BoneNames;

			// Find duplicates in the BulletHitResult array
			for (auto HitResult : BulletHitResult)
			{
				HitActors.AddUnique(HitResult.GetActor()); // Add only the unique items to the HitActors array (no duplicates)
				HitComponents.AddUnique(HitResult.GetComponent()); // Add only the unique items to the HitComponents array (no duplicates)
				BoneNames.AddUnique(HitResult.BoneName);
			}

			for (uint16 i = 0; i < HitActors.Num(); i++)
			{
				HitActor = HitActors[i];
				HitComponent = HitComponents[i];
				BoneName = BoneNames[i];

				if (IsValid(HitActor))
				{
					if (this != HitActor)
					{

						if (HitActor->IsA(AGoblinCharacter::StaticClass()))
						{
							GoblinCharacter = Cast<AGoblinCharacter>(HitActor);
							GoblinCharacter->CurrentHealth -= WeaponDamage;
							GoblinCharacter->bIsGoblinHit = true;
							GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, HitActor->GetName() + " is at " + FString::FromInt(GoblinCharacter->CurrentHealth), true);

							if (GoblinCharacter->CurrentHealth <= 0)
							{
								HitComponent->SetSimulatePhysics(true);
								HitComponent->SetMobility(EComponentMobility::Movable);
								HitComponent->AddImpulseAtLocation(this->GetActorRotation().Vector() * WeaponDamage, HitComponent->GetComponentLocation(), BoneName);

								GoblinCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
								GoblinCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
								GoblinCharacter->GoblinAnimInstance->bHealthBelowZero = true;
							}
						}
						else if (HitActor->IsA(AGruntGoblin::StaticClass()))
						{
							GruntGoblin = Cast<AGruntGoblin>(HitActor);
							GruntGoblin->CurrentHealth -= WeaponDamage;
							GoblinCharacter->bIsGoblinHit = true;
							GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, HitActor->GetName() + " is at " + FString::FromInt(GruntGoblin->CurrentHealth), true);

							if (GruntGoblin->CurrentHealth <= 0)
							{
								HitComponent->SetSimulatePhysics(true);
								HitComponent->SetMobility(EComponentMobility::Movable);
								HitComponent->AddImpulseAtLocation(this->GetActorRotation().Vector() * WeaponDamage, HitComponent->GetComponentLocation(), BoneName);

								GoblinCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
								GoblinCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
								GruntGoblin->GoblinAnimInstance->bHealthBelowZero = true;
							}
						}
					}
				}
			}
		}

		/// Calculate Ammo
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

void AFirstPersonCharacter::CalculateHealth() const
{
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
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AFirstPersonCharacter::OnReload, ReloadTime, false);
	}
}

void AFirstPersonCharacter::Dash()
{
	const FVector Velocity = FVector(((GetInputAxisValue(FName("MoveForward")) * 200) * GetActorForwardVector()) + (GetInputAxisKeyValue(FName("MoveRight")) * 200) * GetActorRightVector());
	
	LaunchCharacter(Velocity * 25, true, true);
}

void AFirstPersonCharacter::MoveXAxis(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), GetInputAxisValue(FName("MoveForward")));
}

void AFirstPersonCharacter::MoveYAxis(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), GetInputAxisValue(FName("MoveRight")));
}

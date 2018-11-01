// Copyright © 2018, AJACKS

#include "FirstPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Materials/MaterialInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "ConstructorHelpers.h"
#include "GoblinCharacter.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
//#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialParameterCollection.h"
#include "DrawDebugHelpers.h"

AFirstPersonCharacter::AFirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Object initializations/loading
	static ConstructorHelpers::FObjectFinder<UClass> AnimationBlueprint(TEXT("AnimBlueprint'/Game/FirstPerson/Animations/FirstPerson_AnimBP.FirstPerson_AnimBP_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> ProjectileBP(TEXT("Class'/Game/FirstPersonBP/Blueprints/FirstPersonProjectile.FirstPersonProjectile_C'"));
	FP_Arms = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'")));
	FP_Gun = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun.SK_FPGun'")));
	TextMaterial = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, TEXT("Material'/Engine/EngineMaterials/DefaultTextMaterialOpaque.DefaultTextMaterialOpaque'")));
	FireMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr, TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'")));
	WeaponFire = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundWave'/Game/FirstPerson/Audio/FirstPersonTemplateWeaponFire02.FirstPersonTemplateWeaponFire02'")));
	ScalarParams = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), nullptr, TEXT("MaterialParameterCollection'/Game/5329_Goblins/Materials/UI/ScalarParams.ScalarParams'")));
	UIMonitor = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/5329_Goblins/Assets/Player/UI_Monitor.UI_Monitor'")));
	BlackMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Player/Black.Black'")));
	CRTBarsMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Player/CRTbars.CRTbars'")));
	DiamondUIMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/UI/DiamondUI_MAT_2.DiamondUI_MAT_2'")));
	ShotgunUIMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/UI/UI_AmmoShell_MAT.UI_AmmoShell_MAT'")));
	FloorPanel = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/5329_Goblins/Assets/Floors/Floor_Panel.Floor_Panel'")));

	// Player default settings
	AutoReceiveInput = EAutoReceiveInput::Player0;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->MaxAcceleration = 5086.0f;

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
	HealthMonitor2 = CreateDefaultSubobject<UStaticMeshComponent>(FName("HealthMonitor2"));
	HealthMonitor2->SetupAttachment(Monitor);
	HealthMonitor2->SetRelativeLocation(FVector(-8.513375f, 4.828093f, -0.414496f));
	HealthMonitor2->SetRelativeRotation(FRotator(-0.000549f, -0.000946f, -179.999939f));
	HealthMonitor2->SetRelativeScale3D(FVector(0.090038f, 0.001000f, 0.074806f));
	HealthMonitor2->SetMobility(EComponentMobility::Movable);
	HealthMonitor2->SetStaticMesh(FloorPanel);
	HealthMonitor2->SetMaterial(0, DiamondUIMaterial);
	HealthMonitor2->bEditableWhenInherited = true;
	
	// Ammo Monitor
	AmmoMonitor2 = CreateDefaultSubobject<UStaticMeshComponent>(FName("AmmoMonitor2"));
	AmmoMonitor2->SetupAttachment(Monitor);
	AmmoMonitor2->SetRelativeLocation(FVector(-2.463829f, 4.856749f, -0.242476f));
	AmmoMonitor2->SetRelativeRotation(FRotator(0.299647f, -0.107086f, -178.468033f));
	AmmoMonitor2->SetRelativeScale3D(FVector(0.122178f, 0.001676f, 0.071209f));
	AmmoMonitor2->SetMobility(EComponentMobility::Movable);
	AmmoMonitor2->SetStaticMesh(FloorPanel);
	AmmoMonitor2->SetMaterial(0, ShotgunUIMaterial);
	AmmoMonitor2->bEditableWhenInherited = true;

	// CRT Bars
	CRTBars2 = CreateDefaultSubobject<UStaticMeshComponent>(FName("CRTBars2"));
	CRTBars2->SetupAttachment(Monitor);
	CRTBars2->SetRelativeLocation(FVector(-8.545621, 4.904549, -1.227119));
	CRTBars2->SetRelativeRotation(FRotator(0.368359, -0.140930, 179.738388));
	CRTBars2->SetRelativeScale3D(FVector(0.172888, 0.000827, 0.084859));
	CRTBars2->SetMobility(EComponentMobility::Movable);
	CRTBars2->SetStaticMesh(FloorPanel);
	CRTBars2->SetMaterial(0, CRTBarsMaterial);
	CRTBars2->bEditableWhenInherited = true;

	// Capsule Component
	GetCapsuleComponent()->SetCapsuleSize(55.0f, 168.0f);

	// Timeline Component
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));

	// Curve Float
	CurveFloat = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/5329_Goblins/CurveFloats/HealthLerpTime.HealthLerpTime'")));

	// Checks
	if (ProjectileBP.Object)
		BlueprintItem = ProjectileBP.Object;
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Attachments
	FirstPersonCamera->AttachToComponent(RootComponent, AttachmentRuleRelative);
	SK_Mesh->AttachToComponent(FirstPersonCamera, AttachmentRuleRelative);
	SK_Gun->AttachToComponent(SK_Mesh, AttachmentRuleRelative, FName("GripPoint"));
	Monitor->AttachToComponent(SK_Gun, AttachmentRuleRelative);
	HealthMonitor2->AttachToComponent(Monitor, AttachmentRuleRelative);
	AmmoMonitor2->AttachToComponent(Monitor, AttachmentRuleRelative);
	CRTBars2->AttachToComponent(Monitor, AttachmentRuleRelative);
	AmmoCounter->AttachToComponent(SK_Gun, AttachmentRuleRelative);
	HealthCounter->AttachToComponent(SK_Gun, AttachmentRuleRelative);
	Sphere->AttachToComponent(SK_Gun, AttachmentRuleRelative);

	// Player setup
	CurrentHealth = MaxHealth;
	HealthCounter->SetText(FText::AsNumber(MaxHealth));
	CurrentAmmo = MaxAmmo;
	AmmoCounter->SetText(FText::AsNumber(MaxAmmo));

	// Loop through all the StaticMeshComponents of the PlayerBP
	//GetComponents<UStaticMeshComponent>(Components);
	//for (int32 i = 0; i < Components.Num(); i++)
	//{
	//	UStaticMeshComponent* StaticMeshComponent = Components[i];
	//	UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
	//}
	//
	//// Set the material we have found from the HealthMonitor Static Mesh Component
	//const auto DiamondMaterial = Components[2]->GetMaterial(0);
	//const auto ShotgunUIMaterial = Components[4]->GetMaterial(0);
	DiamondUIMaterialInstance = UMaterialInstanceDynamic::Create(DiamondUIMaterial, this);
	ShotgunUIMaterialInstance = UMaterialInstanceDynamic::Create(ShotgunUIMaterial, this);
	//Components[2]->SetMaterial(0, DiamondUIMaterialInstance);
	//Components[4]->SetMaterial(0, ShotgunUIMaterialInstance);

	HealthMonitor2->SetMaterial(0, DiamondUIMaterialInstance);
	AmmoMonitor2->SetMaterial(0, ShotgunUIMaterialInstance);

	// Timeline Setup
	FOnTimelineFloat OnTimelineCallback;
	OnTimelineCallback.BindUFunction(this, "OnCalculateHealth");
	LerpSpeed = 5.0f;

	if (CurveFloat != nullptr)
	{
		TimelineComponent = NewObject<UTimelineComponent>(this, FName("HealthLerpSpeed"));
		TimelineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		TimelineComponent->SetPropertySetObject(this);
		TimelineComponent->SetLooping(false);
		TimelineComponent->SetPlaybackPosition(0.0f, false);
		TimelineComponent->SetPlayRate(1.0f / LerpSpeed);
		TimelineComponent->AddInterpFloat(CurveFloat, OnTimelineCallback);
		TimelineComponent->SetTimelineLength(1.0f/ LerpSpeed);
		TimelineComponent->SetTimelineLengthMode(TL_TimelineLength);
		TimelineComponent->RegisterComponent();

		CurveFloat->FloatCurve.Reset();
		CurveFloat->FloatCurve.UpdateOrAddKey(0.0, 0.0f); // Time: 0  Value: 0
		CurveFloat->FloatCurve.UpdateOrAddKey(1.0f / LerpSpeed, 1.0f); // Time: LerpSpeed  Value: 1
	}
}

void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TimelineComponent != nullptr)
		TimelineComponent->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);
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
	InputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AFirstPersonCharacter::Jump);
	InputComponent->BindAction(FName("Jump"), IE_Released, this, &AFirstPersonCharacter::StopJumping);
	InputComponent->BindAction(FName("Fire"), IE_Pressed, this, &AFirstPersonCharacter::Fire);
	InputComponent->BindAction(FName("Reload"), IE_Pressed, this, &AFirstPersonCharacter::Reload);
	InputComponent->BindAction(FName("DEBUG_TakeHealth"), IE_Pressed, this, &AFirstPersonCharacter::CalculateHealth);
}

void AFirstPersonCharacter::Fire()
{
	if (bCanShoot)
	{
		// Play fire animation
		SK_Mesh->GetAnimInstance()->Montage_Play(FireMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f);

		// Spawn projectile and play sound
		AActor *Projectile = GetWorld()->SpawnActor<AActor>(BlueprintItem, FTransform(FQuat(FirstPersonCamera->GetComponentRotation()), FVector(Sphere->GetComponentLocation() + FirstPersonCamera->GetComponentRotation().RotateVector(GunOffset)), FVector(1.0f, 1.0f, 1.0f)));
		UGameplayStatics::PlaySoundAtLocation(this, WeaponFire, GetActorLocation());

		// Calculate Sphere trace
		const float FPCameraPitch = FirstPersonCamera->GetComponentRotation().Pitch;
		const FVector SphereRight = Sphere->GetRightVector();

		StartTrace = Sphere->GetComponentLocation();
		EndTrace = StartTrace + (Sphere->GetRightVector() * WeaponRange);

		const FCollisionShape CapsuleCollision = FCollisionShape::MakeCapsule(SphereTraceRadius, WeaponRange);
		FCollisionObjectQueryParams CollisionParams;
		CollisionParams.AddObjectTypesToQuery(ECC_GameTraceChannel2); // Goblin
		
		//DrawDebugCapsule(GetWorld(), StartTrace + (SphereRight * WeaponRange), WeaponRange, SphereTraceRadius, FQuat::MakeFromEuler(FVector(GetActorRotation().Roll, FPCameraPitch - 90, GetActorRotation().Yaw)), FColor::White, true, -1, 0, 1);

		// Check which actor was hit by the sphere trace and destroy it
		if (GetWorld()->SweepSingleByObjectType(BulletHitResult, StartTrace + (SphereRight * WeaponRange), EndTrace, FQuat::MakeFromEuler(FVector(GetActorRotation().Roll, FPCameraPitch - 90, GetActorRotation().Yaw)), CollisionParams, CapsuleCollision))
		{
			AActor *HitActor = BulletHitResult.GetActor();
			const auto HitComponent = BulletHitResult.GetComponent();

			if (IsValid(HitActor) && HitActor != Projectile && HitComponent != Sphere)
			{
				if (this != HitActor || Projectile != HitActor)
				{
					//HitComponent->SetSimulatePhysics(true);
					//HitComponent->SetMobility(EComponentMobility::Movable);
					//HitComponent->AddImpulseAtLocation(ShootDir * WeaponDamage, BulletHitResult.Location);
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, HitActor->GetName() + " Hit!", true);
					GetWorld()->DestroyActor(HitActor);
				}
			}
		}

		CalculateAmmo();
	}
	else
	{
		//ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor"), FLinearColor(1.0f, 0.0f, 0.025124)); // Update ammo shell color to Red
		//ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor_2"), FLinearColor(1.0f, 0.0f, 0.025124)); // Update ammo shell color to Red
		//ShotgunUIMaterialInstance->SetScalarParameterValue(FName("ScalarAmmo"), 1.0f);
		//ShotgunUIMaterialInstance = Components[4]->CreateAndSetMaterialInstanceDynamic(0); // Set material on the ammo monitor
		//Components[4]->SetMaterial(0, ShotgunUIMaterialInstance);
	}
}

void AFirstPersonCharacter::OnCalculateHealth()
{
	const float TimelineValue = TimelineComponent->GetPlaybackPosition();
	const float CurveFloatValue = CurveFloat->GetFloatValue(TimelineValue); // Get the value of the timeline and feed into the curve float for lerping between health values

	CurrentHealth = FMath::Lerp(float(CurrentHealth), float(NewHealth), CurveFloatValue);
	FMath::Clamp(int(CurrentHealth), 0, int(MaxHealth));

	DiamondUIMaterialInstance->SetScalarParameterValue(FName("ScalarHealth"), float(CurrentHealth) / float(MaxHealth)); // Update health bar

	HealthCounter->SetText(FText::AsNumber(CurrentHealth));

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		return;
	}

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
		FMath::Clamp(int(CurrentHealth) / int(MaxHealth), 0, 100);
		HealthCounter->SetText(FText::AsNumber(CurrentHealth));
	}
}

void AFirstPersonCharacter::OnReload()
{
	if (bIsReloading)
	{
		CurrentAmmo = MaxAmmo;
		AmmoCounter->SetText(FText::AsNumber(MaxAmmo));

		if (GetWorldTimerManager().GetTimerRemaining(ReloadTimerHandle) == 0)
		{
			bCanShoot = true;
			bIsReloading = false;

			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("ScalarAmmo"), float(CurrentAmmo) / float(MaxAmmo)); // Update health bar
			ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor"), FLinearColor(0.714583f, 0.0f, 1.0f)); // Update ammo shell color to Pink
			ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor_2"), FLinearColor(0.714583f, 0.0f, 1.0f)); // Update ammo shell color to Pink

			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString("Reloading done"), true);
		}
	}
}

void AFirstPersonCharacter::CalculateHealth()
{
	NewHealth = CurrentHealth - 10;

	TimelineComponent->PlayFromStart();
}

void AFirstPersonCharacter::CalculateAmmo()
{
	if (bCanShoot)
	{
		CurrentAmmo -= 1;
		AmmoCounter->SetText(FText::AsNumber(CurrentAmmo));


		if (CurrentAmmo <= 0)
		{
			bCanShoot = false;
			AmmoCounter->SetText(FText::AsCultureInvariant("R"));
			ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor"), FLinearColor(1.0f, 0.0f, 0.025124)); // Update ammo shell color to Red
			ShotgunUIMaterialInstance->SetVectorParameterValue(FName("AmmoShellColor_2"), FLinearColor(1.0f, 0.0f, 0.025124)); // Update ammo shell color to Red
			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("ScalarAmmo"), 1.0f); // Update ammo shell
		}
		else
			ShotgunUIMaterialInstance->SetScalarParameterValue(FName("ScalarAmmo"), float(CurrentAmmo) / float(MaxAmmo)); // Update ammo shell
	}
}

void AFirstPersonCharacter::Reload()
{
	if (MaxAmmo == CurrentAmmo)
		bIsFullAmmo = true;
	else
		bIsFullAmmo = false;

	if (!bIsReloading && !bIsFullAmmo)
	{
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AFirstPersonCharacter::OnReload, ReloadTime, false);
		bIsReloading = true;

		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString("Start Reloading"), true);
	}
	//else if (bIsFullAmmo)
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString("Ammo Full"), true);
	//else if (bIsReloading)
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString("Already Reloading"), true);
}

void AFirstPersonCharacter::MoveXAxis(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), GetInputAxisValue(FName("MoveForward")));
}

void AFirstPersonCharacter::MoveYAxis(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), GetInputAxisValue(FName("MoveRight")));

}

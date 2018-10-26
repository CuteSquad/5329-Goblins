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


AFirstPersonCharacter::AFirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Object initializations
	static ConstructorHelpers::FObjectFinder<UClass> AnimationBlueprint(TEXT("AnimBlueprint'/Game/FirstPerson/Animations/FirstPerson_AnimBP.FirstPerson_AnimBP_C'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> ProjectileBP(TEXT("Blueprint'/Game/FirstPersonBP/Blueprints/FirstPersonProjectile.FirstPersonProjectile'"));
	FP_Arms = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'")));
	FP_Gun = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun.SK_FPGun'")));
	TextMaterial = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, TEXT("Material'/Engine/EngineMaterials/DefaultTextMaterialOpaque.DefaultTextMaterialOpaque'")));
	FireMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr, TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'")));
	WeaponFire = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundWave'/Game/FirstPerson/Audio/FirstPersonTemplateWeaponFire02.FirstPersonTemplateWeaponFire02'")));

	// Checks
	if (ProjectileBP.Object)
		BlueprintItem = ProjectileBP.Object->GeneratedClass;

	// Player default settings
	AutoReceiveInput = EAutoReceiveInput::Player0;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->MaxAcceleration = 5086.0f;

	// Camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	FirstPersonCamera->SetWorldLocation(FVector(0.0f, 0.0f, 94.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Skeletal Mesh
	SK_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FP_Mesh"));
	SK_Mesh->AttachToComponent(FirstPersonCamera, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	SK_Mesh->SetRelativeLocation(FVector(-2.0f, -4.5f, -155.0f));
	SK_Mesh->SetRelativeRotation(FRotator(2.0f, -20.0f, 5.0f));
	SK_Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SK_Mesh->AnimClass = AnimationBlueprint.Object;
	SK_Mesh->SetSkeletalMesh(FP_Arms);

	// Gun Skeletal Mesh
	SK_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FP_Gun"));
	SK_Gun->AttachToComponent(SK_Mesh, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	SK_Gun->SetSkeletalMesh(FP_Gun);
	SK_Gun->AttachToComponent(SK_Mesh, FAttachmentTransformRules::KeepRelativeTransform, FName("GripPoint"));

	// Ammo Counter
	AmmoCounter = CreateDefaultSubobject<UTextRenderComponent>(FName("AmmoCounter"));
	AmmoCounter->AttachToComponent(SK_Gun, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	AmmoCounter->SetRelativeLocation(FVector(11.417187, 26.233547, 13.665541));
	AmmoCounter->SetRelativeRotation(FRotator(-19.999941, 89.999878, 3.497171));
	AmmoCounter->SetRelativeScale3D(FVector(-0.131584, -0.141602, 0.116409));
	AmmoCounter->SetText(FString::FromInt(MaxAmmo));
	AmmoCounter->SetWorldSize(32.0f);
	AmmoCounter->SetMaterial(0, TextMaterial);
	AmmoCounter->SetHorizontalAlignment(EHTA_Center);
	AmmoCounter->SetVerticalAlignment(EVRTA_TextCenter);

	// Health Counter
	HealthCounter = CreateDefaultSubobject<UTextRenderComponent>(FName("HealthCounter"));
	HealthCounter->AttachToComponent(SK_Gun, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	HealthCounter->SetRelativeLocation(FVector(20.371742, 25.924501, 11.786989));
	HealthCounter->SetRelativeRotation(FRotator(-20.032705, 90.124283, 4.537992));
	HealthCounter->SetRelativeScale3D(FVector(0.372016, -0.057892, 0.072993));
	HealthCounter->SetText(FString::FromInt(MaxHealth));
	HealthCounter->SetWorldSize(48.0f);
	HealthCounter->SetMaterial(0, TextMaterial);
	HealthCounter->SetHorizontalAlignment(EHTA_Center);
	HealthCounter->SetVerticalAlignment(EVRTA_TextCenter);

	// Sphere
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->AttachToComponent(SK_Gun, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	Sphere->SetSphereRadius(4.0f);
	Sphere->SetRelativeLocation(FVector(0.0f, 56.0f, 11.0f));

	// Capsule Component
	GetCapsuleComponent()->SetCapsuleSize(55.0f, 168.0f);
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	SK_Gun->AttachToComponent(SK_Mesh, FAttachmentTransformRules::KeepRelativeTransform, FName("GripPoint"));
}

void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	SK_Mesh->GetAnimInstance()->Montage_Play(FireMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f);

	AActor *Projectile = GetWorld()->SpawnActor<AActor>(BlueprintItem, FTransform(FQuat(FirstPersonCamera->GetComponentRotation()), FVector(Sphere->GetComponentLocation() + FirstPersonCamera->GetComponentRotation().RotateVector(GunOffset)), FVector(1.0f, 1.0f, 1.0f)));

	UGameplayStatics::PlaySoundAtLocation(this, WeaponFire, GetActorLocation());

	UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraViewPoint(CameraLoc, CameraRot);

	StartTrace = Sphere->GetComponentLocation();
	ShootDir = CameraRot.Vector();

	GetWorld()->LineTraceSingleByChannel(BulletHitResult, StartTrace + FVector::DotProduct(GetActorLocation() - StartTrace, ShootDir) * ShootDir, ShootDir + (ShootDir * WeaponRange), ECC_GameTraceChannel1);

	AActor *HitActor = BulletHitResult.GetActor();
	auto HitComponent = BulletHitResult.GetComponent();

	if (IsValid(HitActor) && HitActor != Projectile && HitComponent != Sphere)
	{
		if (this != HitActor || Projectile != HitActor)
		{
			//HitComponent->AddImpulseAtLocation(ShootDir * WeaponDamage, BulletHitResult.Location);
			UE_LOG(LogTemp, Warning, TEXT("%s Hit!"), *HitActor->GetName());
			//GetWorld()->DestroyActor(HitActor);
		}
	}

	CalculateAmmo();
}

void AFirstPersonCharacter::CalculateHealth()
{
	CurrentHealth -= 5;

	FMath::Clamp(CurrentHealth / MaxHealth, 0, 1);

	if (CurrentHealth <= 0)
	{
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	}

	HealthCounter->SetText(FString::FromInt(CurrentHealth));

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = 100;
		FMath::Clamp(CurrentHealth / MaxHealth, 0, 1);
		HealthCounter->SetText(FString::SanitizeFloat(CurrentHealth));
	}
}

void AFirstPersonCharacter::CalculateAmmo()
{
	if (bCanShoot)
	{
		CurrentAmmo -= 1;
		AmmoCounter->SetText(FString::FromInt(CurrentAmmo));

		if (CurrentAmmo <= 0)
		{
			bCanShoot = false;
			AmmoCounter->SetText(FString("R"));
		}
	}
}

void AFirstPersonCharacter::Reload()
{
	FTimerHandle Handle = FTimerHandle();
	GetWorldTimerManager().SetTimer(Handle, 1.0f, false, 1.0f);

	CurrentAmmo = MaxAmmo;
	AmmoCounter->SetText(FString::FromInt(MaxAmmo));

	bCanShoot = true;
}

void AFirstPersonCharacter::MoveXAxis(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), GetInputAxisValue(FName("MoveForward")));
}

void AFirstPersonCharacter::MoveYAxis(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), GetInputAxisValue(FName("MoveRight")));

}

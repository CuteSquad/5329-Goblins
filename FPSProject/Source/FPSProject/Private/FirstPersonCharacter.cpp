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


AFirstPersonCharacter::AFirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Object initializations
	static ConstructorHelpers::FObjectFinder<UClass> AnimationBlueprint(TEXT("AnimBlueprint'/Game/FirstPerson/Animations/FirstPerson_AnimBP.FirstPerson_AnimBP_C'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> ProjectileBP(TEXT("Blueprint'/Game/FirstPersonBP/Blueprints/FirstPersonProjectile.FirstPersonProjectile'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerBP(TEXT("Blueprint'/Game/5329_Goblins/Blueprints/PlayerBP.PlayerBP'"));
	FP_Arms = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'")));
	FP_Gun = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun.SK_FPGun'")));
	TextMaterial = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, TEXT("Material'/Engine/EngineMaterials/DefaultTextMaterialOpaque.DefaultTextMaterialOpaque'")));
	FireMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr, TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'")));
	WeaponFire = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundWave'/Game/FirstPerson/Audio/FirstPersonTemplateWeaponFire02.FirstPersonTemplateWeaponFire02'")));

	// Checks
	if (ProjectileBP.Object)
		BlueprintItem = ProjectileBP.Object->GeneratedClass;

	if (PlayerBP.Object)
		PlayerBlueprint = PlayerBP.Object->GeneratedClass;

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
	SK_Gun->AttachToComponent(SK_Mesh, AttachmentRules, FName("GripPoint"));

	// Sphere
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->SetupAttachment(SK_Gun);
	Sphere->SetSphereRadius(4.0f);
	Sphere->SetRelativeLocation(FVector(0.0f, 56.0f, 11.0f));

	// Capsule Component
	GetCapsuleComponent()->SetCapsuleSize(55.0f, 168.0f);
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	SK_Gun->AttachToComponent(SK_Mesh, AttachmentRules, FName("GripPoint"));
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
}

void AFirstPersonCharacter::MoveXAxis(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), GetInputAxisValue(FName("MoveForward")));
}

void AFirstPersonCharacter::MoveYAxis(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), GetInputAxisValue(FName("MoveRight")));

}

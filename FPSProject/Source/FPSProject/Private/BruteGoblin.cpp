// Copyright © 2018, AJACKS

#include "BruteGoblin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GoblinAnimInstance.h"
#include "ConstructorHelpers.h"


ABruteGoblin::ABruteGoblin()
{
	static ConstructorHelpers::FObjectFinder<UClass> BruteGoblinAnimBP(TEXT("AnimBlueprint'/Game/5329_Goblins/Blueprints/Animation/BruteGobliAnimBlueprint.BruteGobliAnimBlueprint_C'"));

	BruteGoblinMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/5329_Goblins/Materials/Goblin/BruteGoblin_MAT.BruteGoblin_MAT'")));

	// Health setup
	MaxHealth = 1000;
	CurrentHealth = MaxHealth;

	// Skeletal Mesh
	SK_Goblin = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/5329_Goblins/Assets/Goblin/Brute/BruteGoblin.BruteGoblin'")));
	GetMesh()->SetSkeletalMesh(SK_Goblin);
	GetMesh()->SetMaterial(0, BruteGoblinMaterial);
	GetMesh()->SetWorldLocation(FVector(0, 0, -190));
	GetMesh()->SetWorldScale3D(FVector(1.25f, 1.25f, 1.25f));
	if (BruteGoblinAnimBP.Succeeded())
		GetMesh()->SetAnimInstanceClass(BruteGoblinAnimBP.Object);

	// Other settings
	GetCapsuleComponent()->InitCapsuleSize(110.0f, 195.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600;
	SphereComponent->SetSphereRadius(100.0f);
}

void ABruteGoblin::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetMaterial(0, BruteGoblinMaterial);
}

void ABruteGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If goblin is moving AND has seen the player
	if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
	{
		// Walk but don't attack
		GoblinAnimInstance->bCanWalk = true;
		GoblinAnimInstance->bCanAttack = false;
	}
	// If goblin is not moving AND has seen the player
	else if (GetCharacterMovement()->Velocity == FVector::ZeroVector && GoblinAIController->AIStimulus.WasSuccessfullySensed())
	{
		GoblinAnimInstance->bCanWalk = false;
	}
	// If goblin is not moving
	else if (GetCharacterMovement()->Velocity == FVector::ZeroVector)
		GoblinAnimInstance->bCanWalk = false;
	else
		GoblinAnimInstance->bCanAttack = false;

	if (CurrentHealth <= 0)
	{
		GoblinAnimInstance->bHealthBelowZero = true;
		GoblinAnimInstance->bCanAttack = false;
		GoblinAnimInstance->bCanWalk = false;
	}

	if (bIsGoblinHit)
	{
		if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
		{
			GoblinAnimInstance->bIsStunned = true;
			GoblinAnimInstance->bCanWalk = true;
		}
		else
		{
			GoblinAnimInstance->bIsStunned = false;
			GoblinAnimInstance->bCanWalk = false;
		}
	}
	else
	{
		GoblinAnimInstance->bIsStunned = false;

		if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
		{
			GoblinAnimInstance->bCanWalk = true;
		}
	}
	
}
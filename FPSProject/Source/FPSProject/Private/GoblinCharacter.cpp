// Copyright © 2018, AJACKS

#include "GoblinCharacter.h"
#include "GoblinAIController.h"
#include "Classes/Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConstructorHelpers.h"
#include "GoblinAnimInstance.h"


AGoblinCharacter::AGoblinCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Object loading
	static ConstructorHelpers::FObjectFinder<UClass> GoblinAnimBP(TEXT("AnimBlueprint'/Game/5329_Goblins/Animation/Blueprints/GoblinAnimBlueprint.GoblinAnimBlueprint_C'"));

	// Goblin Character Defaults
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 95.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// Skeletal Mesh
	SK_Goblin = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/5329_Goblins/Assets/Goblin/SKGoblin.SKGoblin'")));
	GetMesh()->SetSkeletalMesh(SK_Goblin);
	GetMesh()->SetWorldLocation(FVector(0, 0, -90));
	GetMesh()->SetWorldScale3D(FVector(1.25, 1.25, 1.25));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel2); // Goblin
	if (GoblinAnimBP.Object != nullptr)
		GetMesh()->SetAnimInstanceClass(GoblinAnimBP.Object);

	// Behavior Tree
	GoblinBehavior = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/5329_Goblins/AI/GoblinBT.GoblinBT'")));

	// AI Controller class setup
	AIControllerClass = AGoblinAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

void AGoblinCharacter::BeginPlay()
{
	Super::BeginPlay();

	GoblinAnimInstance = Cast<UGoblinAnimInstance>(GetMesh()->GetAnimInstance());
	GoblinAIController = Cast<AGoblinAIController>(GetController());
}

void AGoblinCharacter::Tick(float DeltaTime)
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
		// Stop walking and attack
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

void AGoblinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGoblinCharacter::PossessedBy(AController* NewController)
{
	NewController = Cast<AGoblinAIController>(NewController);

	Super::PossessedBy(NewController);
}



// Copyright © 2018, AJACKS

#include "GoblinCharacter.h"
#include "GoblinAIController.h"
#include "Classes/Components/CapsuleComponent.h"


AGoblinCharacter::AGoblinCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Static Mesh
	Goblin = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/5329_Goblins/Assets/GoblinLowPoly.GoblinLowPoly'")));
	if (Goblin != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(Goblin);
		StaticMeshComponent->SetWorldLocation(FVector(0, 0, 140));
		StaticMeshComponent->SetWorldRotation(FRotator(0, -90, -90));
		StaticMeshComponent->SetWorldScale3D(FVector(17, 17, 17));
	}

	// Behavior Tree
	GoblinBehavior = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/5329_Goblins/AI/GoblinBT.GoblinBT'")));

	// AI Controller class setup
	this->AIControllerClass = AGoblinAIController::StaticClass();
	this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Other settings
	GetCapsuleComponent()->InitCapsuleSize(120.0f, 140.0f);
}

void AGoblinCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AGoblinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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



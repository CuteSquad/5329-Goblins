// Copyright © 2018, AJACKS

#include "GoblinCharacter.h"


AGoblinCharacter::AGoblinCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Static Mesh
	Goblin = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'")));
	if (Goblin != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(Goblin);
		StaticMeshComponent->SetWorldLocation(FVector(0, 0, 0));
		StaticMeshComponent->SetWorldScale3D(FVector(0.5, 0.5, 1.75));
	}
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


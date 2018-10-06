// Copyright © 2018, AJACKS

#include "Ladder.h"


ALadder::ALadder()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root
	Root = CreateDefaultSubobject<USceneComponent>(FName("RootScene"));
	RootComponent = Root;

	// Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Static Mesh
	Ladder = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'")));
	if (Ladder != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(Ladder);
		StaticMeshComponent->SetWorldLocation(FVector(0, 0, 0));
		StaticMeshComponent->SetWorldScale3D(FVector(0.25, 1.5, 6.5));
	}

	// Trigger Box Component
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBoxComponent"));
	TriggerBoxComponent->SetupAttachment(StaticMeshComponent);
	TriggerBoxComponent->SetRelativeLocation(FVector(-120, 0, 6));
	TriggerBoxComponent->SetBoxExtent(FVector(85, 50, 55));
	TriggerBoxComponent->SetCollisionProfileName(FName("Trigger"));
	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALadder::ToggleClimbOn);
	TriggerBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALadder::ToggleClimbOff);

	// Other settings
	this->bCanBeDamaged = false;
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();

	OurPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	CharacterMovement = Cast<UCharacterMovementComponent>(OurPawn->GetMovementComponent());
}

void ALadder::ToggleClimbOn(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Climbing mode on"));
	CanClimb = true;
	Climb(HitComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ALadder::ToggleClimbOff(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Climbing mode off"));
	CanClimb = false;
	CharacterMovement->SetMovementMode(MOVE_Walking);
}

void ALadder::Climb(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Climbing..."));
	
	if (CanClimb)
	{
		CharacterMovement->SetMovementMode(MOVE_Flying); // So we can walk upwards
		OurPawn->AddMovementInput(OurPawn->GetActorUpVector(), GetInputAxisValue(FName("MoveForward")));
	}
}

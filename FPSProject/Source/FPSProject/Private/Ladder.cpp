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
	ClimbingTriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBoxComponent"));
	ClimbingTriggerBoxComponent->SetupAttachment(StaticMeshComponent);
	ClimbingTriggerBoxComponent->SetRelativeLocation(FVector(-120, 0, 6));
	ClimbingTriggerBoxComponent->SetBoxExtent(FVector(85, 40, 45));
	ClimbingTriggerBoxComponent->SetCollisionProfileName(FName("Trigger"));
	ClimbingTriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALadder::ToggleClimbOn);
	ClimbingTriggerBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALadder::ToggleClimbOff);

	// Floor Trigger Box Component
	FloorTriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("FloorTriggerBoxComponent"));
	FloorTriggerBoxComponent->SetupAttachment(StaticMeshComponent);
	FloorTriggerBoxComponent->SetRelativeLocation(FVector(-200, 0, -47));
	FloorTriggerBoxComponent->SetBoxExtent(FVector(50, 50, 2));
	FloorTriggerBoxComponent->SetCollisionProfileName(FName("Trigger"));
	FloorTriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALadder::DisableClimbMode);

	// Other settings
	bCanBeDamaged = false;
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();

	OurPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	CharacterMovement = Cast<UCharacterMovementComponent>(OurPawn->GetMovementComponent());
}

void ALadder::ToggleClimbOn(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CanClimb = true;

	AutoReceiveInput = EAutoReceiveInput::Player0;
	InputComponent->BindAxis(FName("MoveForward"), this, &ALadder::MoveForward);

	CharacterMovement->SetMovementMode(MOVE_Flying); // So we can walk up the ladder
}

void ALadder::ToggleClimbOff(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CanClimb = false;

	// Disable receive input and reset axis bindings
	AutoReceiveInput = EAutoReceiveInput::Disabled;
	InputComponent->BindAxis(FName("MoveForward"), this, &ALadder::MoveForward);
	InputComponent->BindAxis(FName("MoveRight"), this, &ALadder::MoveRight);

	CharacterMovement->SetMovementMode(MOVE_Walking);
}

void ALadder::DisableClimbMode(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CanClimb = false;

	// Disable receive input and reset axis bindings
	AutoReceiveInput = EAutoReceiveInput::Disabled;
	InputComponent->BindAxis(FName("MoveForward"), this, &ALadder::MoveForward);
	InputComponent->BindAxis(FName("MoveRight"), this, &ALadder::MoveRight);

	CharacterMovement->SetMovementMode(MOVE_Walking);
}

void ALadder::MoveForward(float Scale)
{
	if (CanClimb)
		OurPawn->AddMovementInput(OurPawn->GetActorUpVector(), GetInputAxisValue(FName("MoveForward"))); // Walk up
	else
		OurPawn->AddMovementInput(OurPawn->GetActorForwardVector(), GetInputAxisValue(FName("MoveForward"))); // Walk normally
}

void ALadder::MoveRight(float Scale)
{
	if (!CanClimb)
		OurPawn->AddMovementInput(OurPawn->GetActorRightVector(), GetInputAxisValue(FName("MoveRight")));
}


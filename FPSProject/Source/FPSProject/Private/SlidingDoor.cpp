// Copyright � 2018, AJACKS

#include "SlidingDoor.h"

ASlidingDoor::ASlidingDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root Component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = Root;

	// Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Static Mesh
	Door = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'")));
	if (Door != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(Door);
		StaticMeshComponent->SetWorldLocation(FVector(0, 0, 0));
		StaticMeshComponent->SetWorldScale3D(FVector(0.5, 4.25, 3.5));
	}

	// Trigger Box Component
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box Component"));
	TriggerBoxComponent->SetupAttachment(StaticMeshComponent);
	TriggerBoxComponent->SetWorldLocation(FVector(-150, 0, 0));
	TriggerBoxComponent->SetBoxExtent(FVector(50, 40, 50));
	TriggerBoxComponent->SetCollisionProfileName(FName("Trigger"));
	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASlidingDoor::Slide);

	// Timeline Component
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));

	// Curve Float
	CurveFloat = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr, TEXT("CurveFloat'/Game/5329_Goblins/DoorSlideTime.DoorSlideTime'")));
	
	// Other settings
	this->bCanBeDamaged = false;
}

// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	// Timeline Setup
	FOnTimelineFloat OnTimelineCallback;

	OnTimelineCallback.BindUFunction(this, "Slide");

	if (CurveFloat != nullptr)
	{
		Timeline = NewObject<UTimelineComponent>(this, FName("DoorAnimation"));
		Timeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		Timeline->SetPropertySetObject(this);
		Timeline->SetLooping(false);
		Timeline->SetPlaybackPosition(0.0f, false);
		Timeline->SetPlayRate(Speed);
		Timeline->AddInterpFloat(CurveFloat, OnTimelineCallback);
		Timeline->SetTimelineLength(Speed);
		Timeline->RegisterComponent();
	}

	// Door
	CurrentLocation = StaticMeshComponent->GetComponentLocation();
}

void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Timeline != nullptr)
		Timeline->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);
}

void ASlidingDoor::Slide(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Timeline->Play();

	const float TimelineValue = Timeline->GetPlaybackPosition();
	const float CurveFloatValue = CurveFloat->GetFloatValue(TimelineValue);

	if (Sideways)
		EndLocation = FVector(CurrentLocation.X, CurrentLocation.Y - YSlide, CurrentLocation.Z);
	else
		EndLocation = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z - ZSlide);

	StaticMeshComponent->SetWorldLocation(FMath::Lerp(CurrentLocation, EndLocation, CurveFloatValue));

	if (IsValid(TriggerBoxComponent))
		TriggerBoxComponent->DestroyComponent();
}

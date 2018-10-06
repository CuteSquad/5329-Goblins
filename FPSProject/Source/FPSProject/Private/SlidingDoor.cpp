// Copyright © 2018, AJACKS

#include "SlidingDoor.h"
#include <functional>

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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Curve Float object found!"));

		CurveFloat = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass(), FName("DoorSlideTime"));
		CurveFloat->FloatCurve.AddKey(0, 0);
		CurveFloat->FloatCurve.AddKey(2.5, 1);
	}

	// Door Setup
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
	const float CurveFloatValue = CurveFloat->GetFloatValue(TimelineValue); // Get the value of the timeline and feed into the curve float for lerping between start and end locations

	if (Sideways)
		EndLocation = FVector(CurrentLocation.X - XSlide, CurrentLocation.Y - YSlide, CurrentLocation.Z); // Minus the X or Y axis to set the end location
	else
		EndLocation = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z - ZSlide); // Minus the Z axis to set the end location

	StaticMeshComponent->SetWorldLocation(FMath::Lerp(CurrentLocation, EndLocation, CurveFloatValue)); // Move the door smoothly from current location to the end location

	if (IsValid(TriggerBoxComponent) && CurveFloat != nullptr)
		TriggerBoxComponent->DestroyComponent(); // Destroy the trigger box once the door has opened.
}

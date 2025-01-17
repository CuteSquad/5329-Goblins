// Copyright © 2018, AJACKS

#include "MovingPlatform.h"
#include "Kismet/GameplayStatics.h"


AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root Component
	Root = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	RootComponent = Root;

	// Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Static Mesh
	Platform = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")));
	if (Platform != nullptr)
	{
		StaticMeshComponent->SetStaticMesh(Platform);
		StaticMeshComponent->SetWorldLocation(FVector(0, 0, 0));
		StaticMeshComponent->SetWorldScale3D(FVector(12, 12, 0.1));
	}

	// Trigger Box Component
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBoxComponent"));
	TriggerBoxComponent->SetupAttachment(StaticMeshComponent);
	TriggerBoxComponent->SetRelativeLocation(FVector(0, 0, 0));
	TriggerBoxComponent->SetBoxExtent(FVector(20, 20, 135));
	TriggerBoxComponent->SetCollisionProfileName(FName("Trigger"));
	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::Move);

	// Timeline Component
	Timeline = CreateDefaultSubobject<UTimelineComponent>(FName("TimelineComponent"));

	// Other settings
	bCanBeDamaged = false;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Timeline Setup
	FOnTimelineFloat OnTimelineCallback;
	OnTimelineCallback.BindUFunction(this, "Move");

	if (CurveFloat != nullptr)
	{
		Timeline = NewObject<UTimelineComponent>(this, FName("PlatformAnimation"));
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

		CurveFloat = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass(), FName("PlatformMoveTime"));
		CurveFloat->FloatCurve.AddKey(0, 0);
		CurveFloat->FloatCurve.AddKey(2.5, 1);
	}

	// Platform Setup
	CurrentLocation = StaticMeshComponent->GetComponentLocation();
	CurrentRotation = StaticMeshComponent->GetComponentRotation();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timeline != nullptr)
		Timeline->TickComponent(DeltaTime, LEVELTICK_TimeOnly, nullptr);
}

void AMovingPlatform::Move(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Timeline->Play();

		if (MovingPlatformSound)
			UGameplayStatics::PlaySoundAtLocation(this, MovingPlatformSound, GetActorLocation());

		if (IsValid(TriggerBoxComponent) && CurveFloat != nullptr)
			TriggerBoxComponent->DestroyComponent(); // Destroy the trigger box once the platform has stopped moving.
	}

	const float TimelineValue = Timeline->GetPlaybackPosition();
	const float CurveFloatValue = CurveFloat->GetFloatValue(TimelineValue); // Get the value of the timeline and feed into the curve float for lerping between start and end location

	EndLocation = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z - ZMove);
	EndRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw - RotateYaw, CurrentRotation.Roll);

	StaticMeshComponent->SetWorldLocation(FMath::Lerp(CurrentLocation, EndLocation, CurveFloatValue));
	StaticMeshComponent->SetWorldRotation(FMath::Lerp(CurrentRotation, EndRotation, CurveFloatValue));

}

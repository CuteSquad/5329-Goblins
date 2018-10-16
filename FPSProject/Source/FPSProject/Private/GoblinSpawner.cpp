// Copyright © 2018, AJACKS

#include "GoblinSpawner.h"
#include "GoblinCharacter.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"


AGoblinSpawner::AGoblinSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Scene Component
	Root = CreateDefaultSubobject<USceneComponent>(FName("Scene Component"));
	Root = RootComponent;
}

void AGoblinSpawner::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::FromInt(Goblins) + FString(" Goblins to spawn"));
	
	GetWorldTimerManager().SetTimer(Handle, this, &AGoblinSpawner::SpawnGoblin, SpawnDelay, true);
}

void AGoblinSpawner::SpawnGoblin()
{
	GetWorld()->SpawnActor<ACharacter>(GoblinType, GetActorLocation(), GetActorRotation());
	GoblinCount++;

	if (GoblinCount == Goblins)
		GetWorldTimerManager().ClearTimer(Handle);
}


// Copyright © 2018, AJACKS

#include "GoblinSpawner.h"
#include "GoblinCharacter.h"


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

	GetWorldTimerManager().SetTimer(Handle, this, &AGoblinSpawner::SpawnGoblin, SpawnDelay, true);
}

void AGoblinSpawner::SpawnGoblin()
{
	GetWorld()->SpawnActor<AGoblinCharacter>(GoblinType, GetActorLocation(), GetActorRotation());
	GoblinCount++;

	if (GoblinCount == Goblins)
		GetWorldTimerManager().ClearTimer(Handle);
}


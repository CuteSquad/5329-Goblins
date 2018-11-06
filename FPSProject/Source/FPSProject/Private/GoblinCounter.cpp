// Copyright © 2018, AJACKS

#include "GoblinCounter.h"
#include "../../../../../../../../../Program Files/Epic Games/UE_4.18/Engine/Plugins/MovieScene/ActorSequence/Source/ActorSequence/Private/ActorSequencePrivatePCH.h"


AGoblinCounter::AGoblinCounter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGoblinCounter::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AGoblinSpawner> SpawnerItr(GetWorld()); SpawnerItr; ++SpawnerItr)
	{
		AGoblinSpawner* Spawner = *SpawnerItr;
		Spawners.Add(Spawner);
	}
}

void AGoblinCounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


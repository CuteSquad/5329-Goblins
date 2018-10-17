// Copyright © 2018, AJACKS

#include "GruntGoblin.h"


AGruntGoblin::AGruntGoblin()
{
	// Static Mesh Component
	StaticMeshComponent->SetWorldLocation(FVector(0, 0, 110));
	StaticMeshComponent->SetWorldRotation(FRotator(0, -90, -90));
	StaticMeshComponent->SetWorldScale3D(FVector(13, 13, 13));

	// Other settings
	GetCapsuleComponent()->InitCapsuleSize(90.0f, 110.0f);
}


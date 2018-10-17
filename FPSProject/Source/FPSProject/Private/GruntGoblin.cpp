// Copyright © 2018, AJACKS

#include "GruntGoblin.h"


AGruntGoblin::AGruntGoblin()
{
	// Static Mesh Component
	StaticMeshComponent->SetWorldLocation(FVector(0, 0, -110));
	StaticMeshComponent->SetWorldScale3D(FVector(1.25, 1.25, 1.25));

	// Other settings
	GetCapsuleComponent()->InitCapsuleSize(90.0f, 130.0f);
}


// Copyright © 2018, AJACKS

#include "GruntGoblin.h"


AGruntGoblin::AGruntGoblin()
{
	// Static Mesh Component
	StaticMeshComponent->SetWorldScale3D(FVector(0.75, 0.75, 1.25));

	// Other settings
	GetCapsuleComponent()->InitCapsuleSize(60.0f, 80.0f);
}


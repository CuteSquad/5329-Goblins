// Copyright © 2018, AJACKS

#include "GruntGoblin.h"
#include "Components/SkeletalMeshComponent.h"


AGruntGoblin::AGruntGoblin()
{
	// Skeletal Mesh
	GetMesh()->SetWorldLocation(FVector(0, 0, -90));
	GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	// Other settings
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);
}


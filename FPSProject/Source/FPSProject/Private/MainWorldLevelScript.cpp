// Copyright © 2018, AJACKS

#include "MainWorldLevelScript.h"
#include "Kismet/GameplayStatics.h"


void AMainWorldLevelScript::BeginPlay()
{
	// Start the camera fade
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(2.0f, 0.0f, 2.5f, FColor::Black);
}

void AMainWorldLevelScript::OnPlayerDeath() const
{
	// Start the camera fade
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(0.0f, 2.0f, 2.0f, FColor::Black, false, true);
}

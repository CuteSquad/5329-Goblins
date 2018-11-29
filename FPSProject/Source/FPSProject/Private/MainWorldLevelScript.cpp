// Copyright © 2018, AJACKS

#include "MainWorldLevelScript.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/Engine.h"


void AMainWorldLevelScript::BeginPlay()
{
	// Start the camera fade
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(2.0f, 0.0f, 5.0f, FColor::Black);

	FTimerHandle InputTimerHandle;
	FTimerDelegate InputTimerDelegate;
	InputTimerDelegate.BindUFunction(this, FName("EnableInput"), UGameplayStatics::GetPlayerController(this, 0));
	GetWorldTimerManager().SetTimer(InputTimerHandle, InputTimerDelegate, 4.0f, false);
}

void AMainWorldLevelScript::OnPlayerDeath() const
{
	// Start the camera fade
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(0.0f, 2.0f, 1.0f, FColor::Black, false, true);
	
	// Restart level after 2 seconds
	FTimerHandle RestartLevelTimerHandle;
	GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, this, &AMainWorldLevelScript::RestartLevel, 2.0f, false);
}

void AMainWorldLevelScript::RestartLevel() const
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));

	if (bShowDebugLevelRestartMessage)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString("Level restarted"));
}

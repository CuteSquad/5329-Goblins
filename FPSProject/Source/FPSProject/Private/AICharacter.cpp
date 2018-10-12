// Copyright © 2018, AJACKS

#include "AICharacter.h"


AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("I'm in the world!"));
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


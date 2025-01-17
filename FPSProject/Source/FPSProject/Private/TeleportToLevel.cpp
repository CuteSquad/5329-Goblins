// Copyright © 2018, AJACKS

#include "TeleportToLevel.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonCharacter.h"
#include "Engine/Engine.h"


ATeleportToLevel::ATeleportToLevel()
{
	PrimaryActorTick.bCanEverTick = false;

	LevelToTeleportTo = FString("BossLevel");

	// Trigger Box Component
	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box Component"));
	TriggerBoxComponent->SetupAttachment(RootComponent);
	TriggerBoxComponent->SetWorldLocation(FVector(0, 0, 0));
	TriggerBoxComponent->SetBoxExtent(FVector(100, 100, 50));
	TriggerBoxComponent->SetCollisionProfileName(FName("Trigger"));
	TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATeleportToLevel::FadeOutScreen);
}

void ATeleportToLevel::BeginPlay()
{
	Super::BeginPlay();

	if (LevelToTeleportTo.IsEmpty())
		LevelToTeleportTo = FString("BossLevel");
}

void ATeleportToLevel::FadeOutScreen(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Player References
	if (OtherActor && OtherActor != this && OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		const auto PlayerCharacter = Cast<AFirstPersonCharacter>(OtherActor);
		auto PlayerHUD = PlayerCharacter->HUD;
		auto PlayerCrosshair = PlayerCharacter->Crosshair;
		
		PlayerHUD->PlayAnimation(PlayerHUD->GoblinsKilledAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		PlayerHUD->PlayAnimation(PlayerHUD->KillBarAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		PlayerHUD->PlayAnimation(PlayerHUD->KillBarOutlineAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		PlayerHUD->PlayAnimation(PlayerHUD->TotalGoblinsAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		PlayerCrosshair->PlayAnimation(PlayerCrosshair->CrosshairAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse);

		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(0.0f, 2.0f, 2.0f, FColor::Black, true, true);

		TeleportDelayTimerDel.BindUFunction(this, FName("Teleport"), LevelToTeleportTo);
		GetWorldTimerManager().SetTimer(TeleportDelayTimerHandle, TeleportDelayTimerDel, 2.0f, false);
	}
}

void ATeleportToLevel::Teleport(FString InMapName) const
{
	UGameplayStatics::OpenLevel(this, FName(*InMapName));
}

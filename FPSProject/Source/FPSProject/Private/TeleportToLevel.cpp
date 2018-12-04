// Copyright © 2018, AJACKS

#include "TeleportToLevel.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonCharacter.h"
#include "Engine/Engine.h"
#include "ConstructorHelpers.h"


ATeleportToLevel::ATeleportToLevel()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UWorld> BossLevel(TEXT("World'/Game/5329_Goblins/Maps/BossLevel.BossLevel'"));

	if (BossLevel.Succeeded())
	{
		NewLevel = BossLevel.Object;
		LevelToTeleportTo = NewLevel;
	}

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

	if (!LevelToTeleportTo)
		LevelToTeleportTo = NewLevel;
}

void ATeleportToLevel::FadeOutScreen(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (LevelToTeleportTo)
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

			TeleportDelayTimerDel.BindUFunction(this, FName("Teleport"), LevelToTeleportTo->GetMapName());
			GetWorldTimerManager().SetTimer(TeleportDelayTimerHandle, TeleportDelayTimerDel, 2.0f, false);
		}
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString("Error: No level selected to teleport to! Please select a map in the details panel"));
}

void ATeleportToLevel::Teleport(FString InMapName) const
{
	UGameplayStatics::OpenLevel(this, FName(*InMapName));
}

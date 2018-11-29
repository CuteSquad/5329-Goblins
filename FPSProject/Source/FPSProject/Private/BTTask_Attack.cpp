// Copyright © 2018, AJACKS

#include "BTTask_Attack.h"
#include "GoblinAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"


UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;

	static ConstructorHelpers::FObjectFinder<UUserDefinedEnum> GoblinAIStateEnum(TEXT("UserDefinedEnum'/Game/5329_Goblins/AI/GoblinAIState.GoblinAIState'"));

	if (GoblinAIStateEnum.Succeeded())
		GoblinAIState = GoblinAIStateEnum.Object;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());
	GoblinCharacter = Cast<AGoblinCharacter>(GoblinController->GetCharacter());
	PlayerCharacter = Cast<AFirstPersonCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));

	GoblinCharacter->GoblinAnimInstance->bCanAttack = true;
	
	return EBTNodeResult::InProgress;

	//AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());
	//
	//APawn *Goblin = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(FName("SelfActor")));
	//APawn *Player = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GoblinController->KeyID[0]));
	//
	//const auto GoblinCharacter = Cast<AGoblinCharacter>(GoblinController->GetPawn());
	//
	//if (Player && GoblinController->AIStimulus.WasSuccessfullySensed())
	//{
	//	GoblinCharacter->GoblinAnimInstance->bCanAttack = true;
	//	return EBTNodeResult::Succeeded;
	//}
	//
	//return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	if (PlayerCharacter->CurrentHealth <= 0)
	{
		GoblinCharacter->GoblinAnimInstance->bCanAttack = false;

		// Set Goblin AI state to Idle
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName(BlackboardKey.SelectedKeyName), GoblinAIState->GetValueByIndex(0));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	//AGoblinAIController *GoblinController = Cast<AGoblinAIController>(OwnerComp.GetAIOwner());
	//const auto GoblinCharacter = Cast<AGoblinCharacter>(GoblinController->GetPawn());
	//const auto PlayerCharacter = Cast<AFirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//
	//FVector SphereForward = GoblinCharacter->SphereComponent->GetForwardVector();
	//FVector StartTrace = GoblinCharacter->SphereComponent->GetComponentLocation();
	//FVector EndTrace = StartTrace + (SphereForward * 100);
	//
	//if (bShowDebugLine)
	//	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::White, true, 2.0f, 0, 5.0f);
	//
	//FCollisionQueryParams CollisionParams;
	//CollisionParams.AddIgnoredActor(OwnerComp.GetAIOwner()->GetPawn());
	//
	//FHitResult HitResult;
	//
	//if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Pawn, CollisionParams))
	//{
	//	const auto HitActor = HitResult.GetActor();
	//
	//	if (IsValid(HitActor))
	//	{
	//		if (GoblinController->GetPawn()->IsA(AGoblinCharacter::StaticClass()))
	//		{
	//			if (HitActor->IsA(AFirstPersonCharacter::StaticClass()))
	//			{
	//				if (PlayerCharacter->CurrentHealth > 0)
	//				{
	//					PlayerCharacter->ApplyDamage(FMath::RandRange(5, 20));
	//				}
	//			}
	//		}
	//	}
	//}
	//
	//GoblinCharacter->bHasAttacked = true;
}

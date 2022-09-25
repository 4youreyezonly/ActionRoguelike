// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VSK_BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"



void UVSK_BTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelatSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DelatSeconds);

	//Check distance between ai pawn and target actor

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bwithRange = Distance < AttackRange;
					bool bHasLos = false;
					if (bwithRange)
					{
						bHasLos = MyController->LineOfSightTo(TargetActor);
					}

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bwithRange&& bHasLos));
				}
			}
		}
	}
}
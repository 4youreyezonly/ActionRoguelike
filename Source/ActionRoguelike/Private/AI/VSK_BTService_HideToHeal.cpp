// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/VSK_BTService_HideToHeal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "VSK_AttributeComponent.h"



void UVSK_BTService_HideToHeal::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelatSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DelatSeconds);

	//Check distance between ai pawn and target actor

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn))
			{
				UVSK_AttributeComponent* AttributeComp = Cast< UVSK_AttributeComponent >(AIPawn->GetComponentByClass(UVSK_AttributeComponent::StaticClass()));

				if (ensure(AttributeComp))
				{
					bool isReady = (((AttributeComp->GetHealth()) / (AttributeComp->GetHealthMax())) < HealLine);
					BlackboardComp->SetValueAsBool(SelfHealKey.SelectedKeyName, isReady);
				}
			}
		}

	}
}


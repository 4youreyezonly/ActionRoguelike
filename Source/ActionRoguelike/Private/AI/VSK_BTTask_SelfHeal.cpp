// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VSK_BTTask_SelfHeal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "VSK_AttributeComponent.h"
#include "AIController.h"


EBTNodeResult::Type UVSK_BTTask_SelfHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

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
					AttributeComp->HealToMax();
				}
				return AttributeComp ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
			}
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;

	}
	return EBTNodeResult::Failed;

}

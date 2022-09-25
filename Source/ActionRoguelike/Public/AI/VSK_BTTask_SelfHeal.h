// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "VSK_BTTask_SelfHeal.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSK_BTTask_SelfHeal : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};

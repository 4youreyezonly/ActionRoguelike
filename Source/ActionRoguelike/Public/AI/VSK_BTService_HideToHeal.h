// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "VSK_BTService_HideToHeal.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSK_BTService_HideToHeal : public UBTService
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector SelfHealKey;

	UPROPERTY(EditAnywhere, Category = "AI")
		float HealLine;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};

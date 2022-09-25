// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "VSK_BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSK_BTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "AI")
		float AttackRange;

protected:

	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector AttackRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};

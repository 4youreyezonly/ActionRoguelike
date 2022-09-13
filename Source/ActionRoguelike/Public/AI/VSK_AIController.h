// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VSK_AIController.generated.h"

class UBehaviorTree;
UCLASS()
class ACTIONROGUELIKE_API AVSK_AIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UBehaviorTree* BehaviorTree;

	virtual void BeginPlay() override;

	
};

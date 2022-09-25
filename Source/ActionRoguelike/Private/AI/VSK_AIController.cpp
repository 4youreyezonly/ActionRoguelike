// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VSK_AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AVSK_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr! Please assign BehaviorTree in your Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}


//	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
//	if (MyPawn)
//	{																																																			+
//		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
//
//		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
//	}

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_Action.h"

void UVSK_Action::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %S"), *GetNameSafe(this));
}

void UVSK_Action::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %S"), *GetNameSafe(this));
}

UWorld* UVSK_Action::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

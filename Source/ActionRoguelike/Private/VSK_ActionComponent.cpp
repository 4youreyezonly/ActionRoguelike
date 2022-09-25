// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_ActionComponent.h"
#include "VSK_Action.h"



UVSK_ActionComponent::UVSK_ActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVSK_ActionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UVSK_ActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UVSK_ActionComponent::AddAction(TSubclassOf<UVSK_Action>ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	UVSK_Action* NewAction = NewObject<UVSK_Action>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UVSK_ActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UVSK_Action* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UVSK_ActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UVSK_Action* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_ActionComponent.h"
#include "VSK_Action.h"
#include "../ActionRoguelike.h"
#include "Net\UnrealNetwork.h"
#include "Engine/ActorChannel.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_VSK);



UVSK_ActionComponent::UVSK_ActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UVSK_ActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UVSK_Action> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}

	}

}

void UVSK_ActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UVSK_Action*> ActionCopy = Actions;
	for (UVSK_Action* Action : ActionCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}


	Super::EndPlay(EndPlayReason);
}


void UVSK_ActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

/*	FString DebugsMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugsMsg;)*/

	for (UVSK_Action* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*GetNameSafe(Action),
			Action->IsRunning()?TEXT("true"): TEXT("false"),
			*GetNameSafe(Action->GetOuter()));
			
		LogOnScreen(GetWorld(), ActionMsg, TextColor, 0.0f);
	}

}

void UVSK_ActionComponent::AddAction(AActor* Instigator,TSubclassOf<UVSK_Action>ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client Attempting to AddAction.[Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}
	UVSK_Action* NewAction = NewObject<UVSK_Action>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{	
		NewAction->Initialize(this);

		Actions.Add(NewAction);
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

bool UVSK_ActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);
	for (UVSK_Action* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

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
			if (Action->IsRunning())
			{

				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}
			
		}
	}
	return false;
}

void UVSK_ActionComponent::RemoveAction(UVSK_Action* ActionToMove)
{
	if (ensure(ActionToMove && !ActionToMove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToMove);
}

void UVSK_ActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UVSK_ActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

UVSK_Action* UVSK_ActionComponent::GetAction(TSubclassOf<UVSK_Action>ActionClass)const
{
	for (UVSK_Action* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	return nullptr;

}

bool UVSK_ActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UVSK_Action* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void UVSK_ActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVSK_ActionComponent,Actions)
}




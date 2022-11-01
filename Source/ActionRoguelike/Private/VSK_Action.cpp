// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_Action.h"
#include "VSK_ActionComponent.h"
#include "../ActionRoguelike.h"
#include "Net\UnrealNetwork.h"

void UVSK_Action::Initialize(UVSK_ActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}
void UVSK_Action::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %S"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	UVSK_ActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
	
	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}


	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}


void UVSK_Action::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %S"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	UVSK_ActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool UVSK_Action::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bIsRunning)
	{
		return false;
	}
	UVSK_ActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

UWorld* UVSK_Action::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

UVSK_ActionComponent* UVSK_Action::GetOwningComponent()const
{
	return ActionComp;
}

void UVSK_Action::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


bool UVSK_Action::IsRunning()const
{
	return RepData.bIsRunning;
}

void UVSK_Action::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVSK_Action, RepData);
	DOREPLIFETIME(UVSK_Action, TimeStarted);
	DOREPLIFETIME(UVSK_Action, ActionComp);
}
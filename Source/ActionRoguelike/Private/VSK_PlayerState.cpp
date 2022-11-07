// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_PlayerState.h"
#include "VSK_SaveGame.h"
#include "Net/UnrealNetwork.h"

int32 AVSK_PlayerState::GetCredits() const
{
	return Credits;
}

void AVSK_PlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AVSK_PlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}
	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
	return true;
}

void AVSK_PlayerState::SavePlayerState_Implementation(UVSK_SaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void AVSK_PlayerState::LoadPlayerState_Implementation(UVSK_SaveGame* SaveObject)
{
	if (SaveObject)
	{
		//Credits = SaveObject->Credits;这个没有触发OnCreditsChanged委托所以不行
		AddCredits(SaveObject->Credits);
	}

}

void AVSK_PlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void AVSK_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVSK_PlayerState, Credits);
}

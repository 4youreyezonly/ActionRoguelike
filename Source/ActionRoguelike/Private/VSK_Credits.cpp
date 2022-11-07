// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_Credits.h"
#include "VSK_PlayerState.h"

// Sets default values
AVSK_Credits::AVSK_Credits()
{
	CreditsAmount = 80;
}

void AVSK_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AVSK_PlayerState* PS = InstigatorPawn->GetPlayerState<AVSK_PlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}






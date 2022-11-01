// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_Powerup_Action.h"
#include "VSK_ActionComponent.h"
#include "VSK_Action.h"

// Sets default values
AVSK_Powerup_Action::AVSK_Powerup_Action()
{

}

void AVSK_Powerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}
	UVSK_ActionComponent* ActionComp = Cast<UVSK_ActionComponent>(InstigatorPawn->GetComponentByClass(UVSK_ActionComponent::StaticClass()));
	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Ation '%s' already know."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}
		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		
	}
}



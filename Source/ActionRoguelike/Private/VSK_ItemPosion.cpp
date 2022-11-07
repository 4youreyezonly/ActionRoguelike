// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_ItemPosion.h"
#include "VSK_AttributeComponent.h"
#include "VSK_PlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

// Sets default values
AVSK_ItemPosion::AVSK_ItemPosion()
{
	CreditCost = 50;
}

FText AVSK_ItemPosion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UVSK_AttributeComponent* AttributeComp = UVSK_AttributeComponent::GetAttributeComponent(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning","Already at full health.");
	}
	return FText::Format(LOCTEXT( "HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}

void AVSK_ItemPosion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UVSK_AttributeComponent* AttributeComp = UVSK_AttributeComponent::GetAttributeComponent(InstigatorPawn);
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AVSK_PlayerState* PS = InstigatorPawn->GetPlayerState<AVSK_PlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				// Only activate if healed successfully
				HideAndCooldownPowerup();
			}
		}
	}

}


#undef LOCTEXT_NAMESPACE

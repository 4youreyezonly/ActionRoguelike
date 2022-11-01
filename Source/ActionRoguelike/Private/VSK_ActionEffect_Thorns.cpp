// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_ActionEffect_Thorns.h"
#include "VSK_AttributeComponent.h"
#include "VSK_GameplayFunctionLibrary.h"
#include "VSK_ActionComponent.h"

UVSK_ActionEffect_Thorns::UVSK_ActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UVSK_ActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UVSK_AttributeComponent* Attributes = UVSK_AttributeComponent::GetAttributeComponent(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UVSK_ActionEffect_Thorns::OnHealthChanged);
	}
}
void UVSK_ActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UVSK_AttributeComponent* Attributes = UVSK_AttributeComponent::GetAttributeComponent(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UVSK_ActionEffect_Thorns::OnHealthChanged);
	}
}

void UVSK_ActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}
		ReflectedAmount = FMath::Abs(ReflectedAmount);
		UVSK_GameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}

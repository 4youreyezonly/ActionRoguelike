// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_AttributeComponent.h"
#include "VSK_GameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("VSK.DamageMultplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UVSK_AttributeComponent::UVSK_AttributeComponent()
{
 
}
#if WITH_EDITOR
void UVSK_AttributeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Health = HealthMax;
}
#endif
UVSK_AttributeComponent* UVSK_AttributeComponent::GetAttributeComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UVSK_AttributeComponent>(FromActor->GetComponentByClass(UVSK_AttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UVSK_AttributeComponent::isActorAlive(AActor* Actor)
{
	UVSK_AttributeComponent* AttributeComponent = GetAttributeComponent(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}



bool UVSK_AttributeComponent::ApplyHealthChange(AActor* instigatorActor,float Delta)
{
	if (!GetOwner()->CanBeDamaged()&&Delta<0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(instigatorActor,this,Health,ActualDelta);

	if (ActualDelta < 0.0F && Health == 0.0f)
	{
		AVSK_GameModeBase* GM = GetWorld()->GetAuthGameMode<AVSK_GameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), instigatorActor);			
		}
	}

	return true;
}

bool UVSK_AttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UVSK_AttributeComponent::IsFullHealthAlive() const
{
	return Health==HealthMax;
}

float UVSK_AttributeComponent::GetHealthMax() const
{
	return HealthMax;
}
float UVSK_AttributeComponent::GetHealth() const
{
	return Health;
}
void UVSK_AttributeComponent::HealToMax()
{
	ApplyHealthChange(nullptr,HealthMax);
}

bool UVSK_AttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_AttributeComponent.h"
#include "VSK_GameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("VSK.DamageMultplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UVSK_AttributeComponent::UVSK_AttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;
	SetIsReplicatedByDefault(true);

}
//#if WITH_EDITOR
//void UVSK_AttributeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	Health = HealthMax;
//}
//#endif
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

bool UVSK_AttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
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
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = NewHealth - OldHealth;
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		if (ActualDelta < 0.0F && Health == 0.0f)
		{
			AVSK_GameModeBase* GM = GetWorld()->GetAuthGameMode<AVSK_GameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0;
}

bool UVSK_AttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}																						                                             

	float OldRage = Rage;
	float NewRage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	float ActualDelta = NewRage - OldRage;
	if (GetOwner()->HasAuthority())
	{
		Rage = NewRage;

		if (ActualDelta != 0.0f)
		{
			MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
		}

		if (ActualDelta < 0.0F && Rage== 0.0f)
		{
			AVSK_GameModeBase* GM = GetWorld()->GetAuthGameMode<AVSK_GameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return ActualDelta != 0;
}

bool UVSK_AttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UVSK_AttributeComponent::IsFullHealth() const
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

void UVSK_AttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVSK_AttributeComponent, Health);
	DOREPLIFETIME(UVSK_AttributeComponent, HealthMax);
	DOREPLIFETIME(UVSK_AttributeComponent, Rage);
	DOREPLIFETIME(UVSK_AttributeComponent, RageMax);

}

void UVSK_AttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this, NewHealth, Delta);
}
void UVSK_AttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

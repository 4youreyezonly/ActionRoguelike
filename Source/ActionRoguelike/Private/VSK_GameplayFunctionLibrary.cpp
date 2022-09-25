// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_GameplayFunctionLibrary.h"
#include "VSK_AttributeComponent.h"


bool UVSK_GameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UVSK_AttributeComponent* AttributeComp = UVSK_AttributeComponent::GetAttributeComponent(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool UVSK_GameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_AttributeComponent.h"

// Sets default values for this component's properties
UVSK_AttributeComponent::UVSK_AttributeComponent()
{
 
	Health = 100;
}




bool UVSK_AttributeComponent::IsAlive()const
{
	return Health > 0.0f;
}

bool UVSK_AttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr,this,Health,Delta);

	return true;
}




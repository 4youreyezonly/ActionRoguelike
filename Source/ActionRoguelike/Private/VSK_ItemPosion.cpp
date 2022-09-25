// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_ItemPosion.h"
#include "VSK_AttributeComponent.h"

// Sets default values
AVSK_ItemPosion::AVSK_ItemPosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

}

void AVSK_ItemPosion::Interact_Implementation(APawn* InstigatorPawn)
{
		UVSK_AttributeComponent* AttributeComp = Cast<UVSK_AttributeComponent>(InstigatorPawn->GetComponentByClass(UVSK_AttributeComponent::StaticClass()));
		if (AttributeComp->IsFullHealthAlive())
		{
			return;
		}
		AttributeComp->ApplyHealthChange(this,HealAmount);
		AddActorLocalOffset(-GetActorUpVector() * 500);
		GetWorldTimerManager().SetTimer(TimerHandle_Heal, this, &AVSK_ItemPosion::Heal_TimeElapsed, Cd);
}

void AVSK_ItemPosion::Heal_TimeElapsed()
{

	AddActorLocalOffset(GetActorUpVector() * 500);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_PowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AVSK_PowerupActor::AVSK_PowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	bIsActive = true;

	// Directly set bool instead of going through SetReplicates(true) within constructor,
	// Only use SetReplicates() outside constructor
	bReplicates = true;
}

void AVSK_PowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}

FText AVSK_PowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void AVSK_PowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void AVSK_PowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AVSK_PowerupActor::ShowPowerup, RespawnTime);
}

void AVSK_PowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	 
	OnRep_IsActive();
}


void AVSK_PowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
}


void AVSK_PowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVSK_PowerupActor, bIsActive);
}



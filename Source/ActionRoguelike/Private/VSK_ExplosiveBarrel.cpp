// Fill out your copyright notice in the Description page of Project Settings.

#include "VSK_ExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AVSK_ExplosiveBarrel::AVSK_ExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);
}

// Called when the game starts or when spawned
void AVSK_ExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVSK_ExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


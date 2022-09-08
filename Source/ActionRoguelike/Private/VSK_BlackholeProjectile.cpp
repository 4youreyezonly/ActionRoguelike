// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_BlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AVSK_BlackholeProjectile::AVSK_BlackholeProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MovementComp->InitialSpeed = 550.0f;
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(SphereComp);

}																	   

// Called when the game starts or when spawned
void AVSK_BlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AVSK_BlackholeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Fill out your copyright notice in the Description page of Project Settings.


#include "VSK_DashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AVSK_DashProjectile::AVSK_DashProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MovementComp->InitialSpeed = 2000.0f;

}

// Called when the game starts or when spawned
void AVSK_DashProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AVSK_DashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





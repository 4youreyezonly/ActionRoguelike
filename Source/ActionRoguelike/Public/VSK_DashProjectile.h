// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VSK_Projectile.h"
#include "VSK_DashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AVSK_DashProjectile : public AVSK_Projectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVSK_DashProjectile();

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:
	// Called every frame
	virtual void Tick(float DeltaTime);
	
};

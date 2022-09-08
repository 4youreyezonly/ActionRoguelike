// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VSK_Projectile.h"
#include "VSK_BlackholeProjectile.generated.h"


class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API AVSK_BlackholeProjectile : public AVSK_Projectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVSK_BlackholeProjectile();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		URadialForceComponent* RadialForceComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:
	// Called every frame
	virtual void Tick(float DeltaTime);
	
};

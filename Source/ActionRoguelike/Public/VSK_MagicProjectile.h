// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VSK_Projectile.h"
#include "VSK_MagicProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE_API AVSK_MagicProjectile : public AVSK_Projectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVSK_MagicProjectile();

protected:
	
	UFUNCTION()
		void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime);

};

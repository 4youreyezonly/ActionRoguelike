// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VSK_Projectile.h"
#include "Particles/ParticleEmitter.h"
#include "GameplayTagContainer.h"
#include "VSK_MagicProjectile.generated.h"
class UParticleSystem;
class USoundBase;
class UVSK_ActionEffect;


UCLASS()
class ACTIONROGUELIKE_API AVSK_MagicProjectile : public AVSK_Projectile
{
	GENERATED_BODY()
	
protected:	

	UPROPERTY(EditAnywhere,Category="Attack")
		float DamageAmount;

	UPROPERTY(EditAnywhere, Category = "Attack")
		float RageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		FGameplayTag ParryTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		TSubclassOf<UVSK_ActionEffect> BurningActionClass;

	UFUNCTION()
		void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;

public:	

   	AVSK_MagicProjectile();
};

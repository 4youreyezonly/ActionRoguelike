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
	// Sets default values for this actor's properties
 	// Called every frame
	virtual void Tick(float DeltaTime);

	UPROPERTY(EditAnywhere,Category="Attack")
		float DamageAmount;

	UPROPERTY(EditAnywhere, Category = "Attack")
		float RageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")

		UParticleSystem* ExplodeFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		class USoundBase* HitSound;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		FGameplayTag ParryTag;
	
	UFUNCTION()
		void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		TSubclassOf<UVSK_ActionEffect> BurningActionClass;


	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:	

   	AVSK_MagicProjectile();
};

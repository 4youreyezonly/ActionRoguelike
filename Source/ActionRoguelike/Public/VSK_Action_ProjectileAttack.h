// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VSK_Action.h"
#include "VSK_Action_ProjectileAttack.generated.h"

/**
 * 
 */
class AttackAnim;
class CastingEffect;
class AVSK_Character;

UCLASS()
class ACTIONROGUELIKE_API UVSK_Action_ProjectileAttack : public UVSK_Action
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
		FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
		float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UParticleSystem* CastingEffect;
	UFUNCTION()
		void AttackDelay_Elapsed(AVSK_Character* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator)override;

	UVSK_Action_ProjectileAttack();

};


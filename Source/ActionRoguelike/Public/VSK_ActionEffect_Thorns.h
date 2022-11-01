// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VSK_ActionEffect.h"
#include "VSK_ActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSK_ActionEffect_Thorns : public UVSK_ActionEffect
{
	GENERATED_BODY()

public:
	UVSK_ActionEffect_Thorns();

	virtual void StartAction_Implementation(AActor* Instigator)override;

	virtual void StopAction_Implementation(AActor* Instigator)override;	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
		float ReflectFraction;

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delta);
};

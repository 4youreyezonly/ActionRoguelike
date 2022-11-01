// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VSK_Action.h"
#include "VSK_ActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSK_ActionEffect : public UVSK_Action
{
	GENERATED_BODY()

public:
	UVSK_ActionEffect();

	virtual void StartAction_Implementation(AActor* Instigator)override;

	virtual void StopAction_Implementation(AActor* Instigator)override;

	UFUNCTION(BlueprintCallable, Category = "Action")
		float GetTimeRemaining()const;




protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
		float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
		float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
		void ExecutePeriodEffect(AActor* Instigator);




	
};

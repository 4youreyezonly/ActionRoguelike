// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VSK_AnimInstance.generated.h"

class UVSK_ActionComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UVSK_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Animation")
		bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		UVSK_ActionComponent* ActionComp;

	void NativeInitializeAnimation()override;

	void NativeUpdateAnimation(float DeltaSeconds)override;
};

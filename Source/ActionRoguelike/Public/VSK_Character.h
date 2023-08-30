// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VSK_Character.generated.h"

//#define ECC_AIPawn ECC_GameTraceChannel1

class UVSK_InteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UVSK_AttributeComponent;
class UVSK_ActionComponent;

UCLASS()
class ACTIONROGUELIKE_API AVSK_Character : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
		UVSK_InteractionComponent* InterActionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
		UVSK_AttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UVSK_ActionComponent* ActionComp;

	void MoveForward(float value);

	void MoveRight(float value);

	void PrimaryAttack();

	void UltimateAttack();

	void TransAttack();

	void SprintStart();

	void SprintStop();

	void JumpStart();														    

	void JumpEnd();

	void PrimaryInteract();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;


public:

	AVSK_Character();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	virtual FVector GetPawnViewLocation() const override;



};
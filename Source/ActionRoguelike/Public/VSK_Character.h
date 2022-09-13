// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VSK_Character.generated.h"

class UVSK_InteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UVSK_AttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AVSK_Character : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> CommonProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> UltimateProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> TransProjectileClass;

	UPROPERTY(EditAnywhere,Category = "Attack")
		UAnimMontage* CommonAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* UltimateAttackAnim;


	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_UltimateAttack;

	FTimerHandle TimerHandle_TransAttack;



public:
	// Sets default values for this character's properties
	AVSK_Character();

protected:

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
		UVSK_InteractionComponent* InterActionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
		UVSK_AttributeComponent* AttributeComp;



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void UltimateAttack();
	void UltimateAttack_TimeElapsed();
	void TransAttack();
	void TransAttack_TimeElapsed();

	//��Ծ��ʼ
	void JumpStart();
	//��Ծ����
	void JumpEnd();

	void PrimaryInteract();
	
	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delat);

	virtual void PostInitializeComponents() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
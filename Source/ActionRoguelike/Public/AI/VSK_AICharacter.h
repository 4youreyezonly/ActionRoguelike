// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VSK_AICharacter.generated.h"
class UPawnSensingComponent;
class UVSK_AttributeComponent;
class UUserWidget;
class UVSK_WorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API AVSK_AICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AVSK_AICharacter();

protected:

	UVSK_WorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	void SetTargetActor(AActor* NewTarget);

	virtual void PostInitializeComponents()override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UVSK_AttributeComponent* AttributeComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info, meta = (AllowPrivateAccess = "true"))
	//	class UWidgetComponent* WidgetComponent;

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
		void OnPawnSeen(APawn* Pawn);

	virtual void Tick(float DeltaTime) override;

};
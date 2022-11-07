// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSK_GameplayInterface.h"
#include "VSK_PowerupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API AVSK_PowerupActor : public AActor, public IVSK_GameplayInterface
{
	GENERATED_BODY()
protected:

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
		bool bIsActive;

	UFUNCTION()
		void OnRep_IsActive();

	UPROPERTY(EditAnywhere, Category = "Powerup")
		float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
		void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

public:

	AVSK_PowerupActor();

};

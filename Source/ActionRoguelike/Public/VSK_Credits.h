// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSK_GameplayInterface.h"
#include "VSK_Credits.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AVSK_Credits : public AActor, public IVSK_GameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVSK_Credits();

protected:

	void Interact_Implementation(APawn* InstigatorPawn);

 	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category ="Credits")
		float CreditsAmount;

	void Credits_TimeElapsed();

	FTimerHandle TimerHandle_Heal;

	UPROPERTY(EditAnywhere ,Category = "Credits")
		float Cd;

public:	



};

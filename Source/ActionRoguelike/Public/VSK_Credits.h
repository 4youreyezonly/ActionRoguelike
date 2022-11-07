// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VSK_PowerupActor.h"
#include "VSK_Credits.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AVSK_Credits : public AVSK_PowerupActor
{
	GENERATED_BODY()



protected:


	UPROPERTY(EditAnywhere, Category = "Credits")
		int32 CreditsAmount;


public:
	AVSK_Credits();

	void Interact_Implementation(APawn* InstigatorPawn) override;
};

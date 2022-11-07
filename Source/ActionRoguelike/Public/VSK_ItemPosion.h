// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VSK_PowerupActor.h"
#include "VSK_ItemPosion.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AVSK_ItemPosion : public AVSK_PowerupActor
{
	GENERATED_BODY()

	
public:	
	void Interact_Implementation(APawn* InstigatorPawn);

	AVSK_ItemPosion();

	FText GetInteractText_Implementation(APawn* InstigatorPawn);


protected:


	UPROPERTY(EditAnywhere)
		int32 CreditCost;

};

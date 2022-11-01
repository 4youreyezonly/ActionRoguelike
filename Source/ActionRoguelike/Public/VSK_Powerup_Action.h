// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSK_GameplayInterface.h"
#include "VSK_Powerup_Action.generated.h"
class UVSK_Action;
UCLASS()
class ACTIONROGUELIKE_API AVSK_Powerup_Action : public AActor, public IVSK_GameplayInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn)override;
	
public:	
	// Sets default values for this actor's properties
	AVSK_Powerup_Action(); 

protected:
	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<UVSK_Action> ActionToGrant;

};

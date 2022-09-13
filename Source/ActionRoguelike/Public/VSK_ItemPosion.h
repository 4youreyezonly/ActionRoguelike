// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSK_GameplayInterface.h"
#include "VSK_ItemPosion.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AVSK_ItemPosion : public AActor, public IVSK_GameplayInterface
{
	GENERATED_BODY()
	void Interact_Implementation(APawn* InstigatorPawn);
	
public:	
	// Sets default values for this actor's properties
	AVSK_ItemPosion();

	UPROPERTY(EditAnywhere)
	float HealAmount;

	UPROPERTY(EditAnywhere)
	float Cd;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	FTimerHandle TimerHandle_Heal;

	void Heal_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

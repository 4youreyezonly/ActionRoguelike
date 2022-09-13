// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSK_TargetDummy.generated.h"

class  UVSK_AttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AVSK_TargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVSK_TargetDummy();

protected:
	UPROPERTY(VisibleAnywhere)
		UVSK_AttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, UVSK_AttributeComponent* OwningComp, float NewHealth, float Delat);

};

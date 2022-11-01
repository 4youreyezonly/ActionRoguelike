// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VSK_InteractionComponent.generated.h"

class UVSK_WorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UVSK_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	UVSK_InteractionComponent();

protected:

	UFUNCTION(Server,Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UVSK_WorldUserWidget>DefaultWidgetClass;

	UPROPERTY()
		UVSK_WorldUserWidget* DefaultWidgetInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

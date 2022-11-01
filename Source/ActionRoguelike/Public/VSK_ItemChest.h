// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VSK_GameplayInterface.h"
#include "VSK_ItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AVSK_ItemChest : public AActor, public IVSK_GameplayInterface
{
	GENERATED_BODY()


	
public:	
	UPROPERTY(EditAnywhere)
	float TargetPitch;
	// Sets default values for this actor's properties
	AVSK_ItemChest();

	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation();

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly,SaveGame)//RepNotify
		bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		UStaticMeshComponent* LidMesh;

};
